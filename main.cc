#include "bst.hpp"

#include <iostream>
#include <any>
#include <string>
#include <variant>
#include <vector>
#include <type_traits>
#include <optional>
#include <sstream>
#include <limits>
#include <cctype>
#include <cstdlib>

namespace
{
    std::string testPath = "";
}

inline std::string trim(const std::string &s)
{
    constexpr char whitespace[] = " \t\n\r";
    const size_t first = s.find_first_not_of(whitespace);

    return (first != std::string::npos) ? s.substr(first, (s.find_last_not_of(whitespace) - first + 1)) : std::string{};
}

template <typename T = int>
bool startsWithDigit(const std::string &s)
{
    if (s.empty())
        return false;

    if (std::isdigit(s.front()))
        return true;

    return (((std::is_signed<T>::value && (s.front() == '-')) || (s.front() == '+')) && ((s.size() > 1) && std::isdigit(s[1])));
}

template <typename T = int>
std::optional<T> stonum(const std::string &st)
{
    const auto s = trim(st);
    bool ok = startsWithDigit<T>(s);

    auto v = T{};

    if (ok)
    {
        std::istringstream ss(s);

        ss >> v;
        ok = (ss.peek() == EOF);
    }

    return ok ? v : std::optional<T>{};
}

std::optional<std::string> getline(std::istream &is, const std::string &def = "")
{
    for (auto no = is.rdbuf()->in_avail(); no && is && std::isspace(is.peek()); is.ignore(), --no)
        ;

    std::string ln;

    return std::getline(is, ln) ? (ln.empty() && !def.empty() ? def : ln) : (is.clear(), std::optional<std::string>{});
}

auto getline(const std::string &prm = "", const std::string &def = "")
{
    std::optional<std::string> o;

    do
    {
        std::cout << prm;
        if (!def.empty())
            std::cout << " [" << def << "]";

        std::cout << " :";
        o = getline(std::cin, def);
    } while (!o.has_value() && (std::cout << "Invalid input" << std::endl));

    return *o;
}

template <typename T = std::string>
std::optional<T> getdata(std::istream &is)
{
    auto i = T{};
    const bool b = (is >> i) && std::isspace(is.peek());

    for (is.clear(); is && !std::isspace(is.peek()); is.ignore())
        ;
    return b ? i : std::optional<T>{};
}

template <typename T = int>
auto getnum(std::istream &is, bool wholeline = true)
{
    if (wholeline)
    {
        const auto o = getline(is);
        return o.has_value() ? stonum<T>(*o) : std::optional<T>{};
    }

    return getdata<T>(is);
}

template <typename T = int>
auto getnum(const std::string &prm = "", T nmin = std::numeric_limits<T>::lowest(), T nmax = std::numeric_limits<T>::max(), bool wholeline = true)
{
    const auto showdefs = [nmin, nmax]()
    {
        std::cout << " (";

        if (nmin != std::numeric_limits<T>::lowest() || std::is_unsigned<T>::value)
            std::cout << nmin;

        std::cout << " - ";

        if (nmax != std::numeric_limits<T>::max())
            std::cout << nmax;

        std::cout << ")";
    };

    std::optional<T> o;

    do
    {
        std::cout << prm;

        if ((nmin != std::numeric_limits<T>::lowest()) || (nmax != std::numeric_limits<T>::max()))
            showdefs();

        std::cout << " :";
        o = getnum<T>(std::cin, wholeline);
    } while ((!o.has_value() || (((*o < nmin) || (*o > nmax)))) && (std::cout << "Invalid input" << std::endl));

    return *o;
}

std::optional<char> getchr(std::istream &is, char def = 0, bool wholeline = true)
{
    if (wholeline)
    {
        if (auto o = getline(is); o.has_value())
        {
            return (o->empty() && def ? def : ((o->size() == 1) ? o->front() : std::optional<char>{}));
        }
        return {};
    }
    return getdata<char>(is);
}

auto getchr(const std::string &prm = "", const std::string &valid = "", char def = 0, bool wholeline = true)
{
    const auto showopt = [&valid, def]()
    {
        std::cout << " (";

        for (size_t i = 0, s = valid.size(); i < s; ++i)
            std::cout << (i ? "/" : "") << valid[i];

        if (std::cout << ")"; def)
            std::cout << " [" << def << "]";
    };

    std::optional<char> o;

    do
    {
        if (std::cout << prm; !valid.empty())
            showopt();

        std::cout << " :";
        o = getchr(std::cin, def, wholeline);
    } while ((!o.has_value() || ((!valid.empty()) && (valid.find(*o) == std::string::npos))) && (std::cout << "Invalid input" << std::endl));

    return *o;
}

class Menu
{
private:
    using f_type = void (*)(std::any &param);

    struct MenItm
    {
        std::string name;
        std::variant<f_type, Menu *> func;
    };

    using vmi = std::vector<MenItm>;

public:
    Menu() {}
    Menu(const std::string &t, const vmi &vm) : stitle(t), mitems(vm) {}

    auto title() const noexcept
    {
        return stitle;
    }

    void title(const std::string &t)
    {
        stitle = t;
    }

    void menu(std::any &param)
    {
        menu(*this, param);
    }

    bool erase(size_t indx)
    {
        if (indx < mitems.size())
        {
            mitems.erase(mitems.begin() + indx);
            return true;
        }

        return false;
    }

    bool append(const MenItm &mi)
    {
        mitems.emplace_back(mi);
        return true;
    }

    bool insert(size_t indx, const MenItm &mi)
    {
        if (indx < mitems.size())
        {
            mitems.insert(mitems.begin() + indx, mi);
            return true;
        }

        return false;
    }

private:
    class RunVisitor
    {
    public:
        RunVisitor(std::any &par) : param(par) {}

        void operator()(f_type func) { func(param); }
        void operator()(Menu *menu) { Menu::menu(*menu, param); }

    private:
        std::any &param;
    };

    static void menu(const Menu &m, std::any &param)
    {
        const static auto show = [](const Menu &mu)
        {
            std::ostringstream oss;
            const auto nom = mu.mitems.size();

            oss << '\n'
                << mu.title() << "\n\n";

            for (size_t i = 0U; i < nom; ++i)
                oss << i + 1 << ")  " << mu.mitems[i].name << '\n';

            oss << "0)  Wyjscie z menu\n\nWybierz numer opcji menu";
            return getnum<size_t>(oss.str(), 0, nom);
        };

        for (size_t opt = 0U; (opt = show(m)) > 0;)
            std::visit(RunVisitor(param), m.mitems[opt - 1].func);
    }

    std::string stitle;
    vmi mitems;
};

using namespace std;
using Params = vector<variant<size_t, int, double, char, string>>;

void f1([[maybe_unused]] any &param)
{
    auto num = getnum<size_t>("Wprowadz ile liczb ma byc w drzewie");
    std::unique_ptr<bst::Node<int>> node;
    for (std::size_t i = 0; i < num; ++i)
    {
        node = bst::insertNode<int>(std::move(node), std::rand() % 100);
    }
    bst::print(node);
}

void f2([[maybe_unused]] any &param)
{
    auto num = getnum<size_t>("Wprowadz ile liczb ma byc w drzewie");
    auto numToFind = getnum<size_t>("Wprowadz liczbe ktora chcesz znalezc");
    std::unique_ptr<bst::Node<int>> node;
    for (std::size_t i = 0; i < num; ++i)
    {
        node = bst::insertNode<int>(std::move(node), std::rand() % 100);
    }
    bst::print(node);
    if (bst::search(node.get(), static_cast<int>(numToFind)) != nullptr)
    {
        std::cout << "Znaleziono wartosc: " << numToFind << " w drzewie\n";
        return;
    }
    std::cout << "Nie znaleziono wartosci: " << numToFind << " w drzewie\n";
}

void f6(any &param)
{
    auto &v = any_cast<Params &>(param);

    v.push_back(getnum<double>("Enter a real between", 5.5, 50.5));
}

void f3([[maybe_unused]] any &param)
{
    std::system(testPath.c_str());
}

void f7(any &param)
{
    auto &v = any_cast<Params &>(param);

    v.push_back(getchr("Enter a vowel", "aeiou", 'a'));
}
void f4(any &param)
{
    auto &v = any_cast<Params &>(param);

    v.push_back(getline("Enter text"));
}

void f51(any &param)
{
    const static auto proc = [](const auto &val)
    { cout << val << endl; };

    auto &v = any_cast<Params &>(param);

    cout << "Entered data is\n";

    for (const auto &d : v)
        visit(proc, d);
}

void f5(any &param)
{
    auto &v = any_cast<Params &>(param);

    cout << "Entered data is\n";

    for (const auto &d : v)
        if (auto pvi = get_if<int>(&d))
            cout << *pvi << endl;
        else if (auto pvd = get_if<double>(&d))
            cout << *pvd << endl;
        else if (auto pvc = get_if<char>(&d))
            cout << *pvc << endl;
        else if (auto pvs = get_if<string>(&d))
            cout << *pvs << endl;
        else if (auto pvu = get_if<size_t>(&d))
            cout << *pvu << endl;
        else
            cout << "Unknown type" << endl;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Zla liczba argumentow. Nalezy podac jeden argument!\n";
        return -1;
    }
    std::cout << argc << '\n'
              << argv[1] << '\n';

    Menu m4{"Zaawansowany przyklad"s, {{"Znajdz liczbe w drzewie"s, f2}}};
    Menu m3{"Trywialny przyklad"s, {{"Podaj liczbe elementow do dodania do drzewa"s, f1}}};
    Menu m2{"Uruchomienie scenariuszy testowych"s, {{"Uruchom testy"s, f3}}};
    Menu m1{"Manu glowne"s, {{"Trywialny przyklad", &m3}, {"Zaawansowany przyklad"s, &m4}, {"Uruchomienie scenariuszy testowych"s, &m2}}};

    any param = Params{};
    std::stringstream ss;
    ss << argv[1] << " --list-tests && " << argv[1] << " -s";
    testPath = ss.str();
    m1.menu(param);
}