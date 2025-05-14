#include <iostream>
#include <stdexcept> // Для std::out_of_range, std::invalid_argument

using namespace std;

//Абстрактний клас, який має оператор для зміни рядків типу MyString // Динамічний поліморфізм
template <typename T>
class Transformer {
public:
    virtual T operator()(const T& ch) const = 0;
    virtual ~Transformer() = default;
};

// Налаштування для динамічної зміни рядка
class ToUpper : public Transformer<char> {
public:
    char operator()(const char& ch) const override {
        return std::toupper(ch);
    }
};
// Налаштування для динамічної зміни рядка
class ToLower : public Transformer<char> {
public:
    char operator()(const char& ch) const override {
        return std::tolower(ch);
    }
};
// Налаштування для динамічної зміни рядка
class ToStar : public Transformer<char> {
public:
    char operator()(const char& ch) const override {
        return '*';
    }
};

//Абстрактний клас, який має оператор для зміни рядків типу MyString // Статичний поліморфізм
struct Shift {
    char operator()(char ch) const {
        return ch + 1;
    }
};
struct ToX {
    char operator()(char ch) const {
        return 'x';
    }
};


template <typename T>
class MyString {
private:
    int strLength; // Перейменовано length
    T* data;

public:
    // За замовчуванням
    MyString() {
        strLength = 0;
        data = new T[1];
        data[0] = T();
    }

    // Конструктор копіювання
    MyString(const MyString& copyString) {
        strLength = copyString.strLength;
        data = new T[strLength + 1];
        for (int i = 0; i < strLength; ++i) { // Явно вказано int
            data[i] = copyString.data[i];
        }
        data[strLength] = T();
    }

    // Конструктор ініціалізації рядка однаковим символом
    MyString(T charSymbol, int leng) {
        if (leng > 0) {
            this->strLength = leng;
            this->data = new T[strLength + 1];
            for (int i = 0; i < strLength; i++) { // Явно вказано int
                data[i] = charSymbol;
            }
            data[strLength] = T();
        }
        else {
            this->strLength = 0;
            this->data = new T[1];
            data[0] = T();
        }
    }

    // Конструктор переміщення
    MyString(MyString&& other) {
        this->data = other.data;
        this->strLength = other.strLength;

        other.data = nullptr;
        other.strLength = 0;
    }

    // Ініціалізація вказівником на масив
    MyString(const T* str) {
        this->strLength = 0;
        for (int i = 0; str[i] != T(); i++) { // Явно вказано int
            this->strLength++;
        }
        data = new T[strLength + 1];
        for (int i = 0; i < strLength; i++) { // Явно вказано int
            data[i] = str[i];
        }
        data[strLength] = T();
    }

    // Ініціалізація двома вказівниками (begin - end)
    MyString(const T* begin, const T* end) {
        if (end == begin) {
            this->strLength = 0;
            this->data = new T[1];
            data[0] = T();
        }
        if (begin > end) {
            throw std::invalid_argument("Begin pointer is after end pointer");
        }
        this->strLength = end - begin;
        this->data = new T[strLength + 1];
        for (int i = 0; i < strLength; i++) { // Явно вказано int
            data[i] = begin[i];
        }
        data[strLength] = T();
    }

    // Конструктор із рядка з іншим типом
    template<typename OtherType>
    MyString(const OtherType* other) {
        this->strLength = 0;
        for (int i = 0; other[i] != OtherType(); i++) { // Явно вказано int
            strLength++;
        }
        this->data = new T[strLength + 1];
        for (int i = 0; i < strLength; i++) { // Явно вказано int
            data[i] = static_cast<T>(other[i]);
        }
        data[strLength] = T();
    }


    /////////////////////////////////////////////////////////////////////////////////////  Operators
    // Оператор присвоєння
    MyString& operator=(const MyString& other) { // Повертаємо посилання
        if (this == &other) return *this;
        delete[] data; // Виправлено синтаксис
        this->strLength = other.strLength;
        data = new T[strLength + 1];
        for (int i = 0; i < strLength; i++) { // Явно вказано int
            this->data[i] = other.data[i];
        }
        this->data[strLength] = T();
        return *this; // Повертаємо посилання
    }

    // Оператор переміщення
    MyString& operator=(MyString&& other) {
        if (this == &other) return *this;
        this->strLength = other.strLength;
        delete[] data; // Виправлено синтаксис
        this->data = other.data;

        other.data = nullptr;
        other.strLength = 0;
        return *this;
    }

    // Оператор індексу
    T& operator[](int pos) {
        if (pos >= 0 && pos < strLength) {
            return data[pos];
        }
        else {
            throw std::out_of_range("Index out of bounds");
        }
    }

    const T& operator[](int pos) const {
        if (pos >= 0 && pos < strLength) {
            return data[pos];
        }
        else {
            throw std::out_of_range("Index out of bounds");
        }
    }
    // Оператор конкатинації одного символа
    MyString& operator+=(T ch) {
        T* newData = new T[this->strLength + 2]; // +1 для нульового символу

        // Копіюємо існуючі символи
        for (int i = 0; i < strLength; ++i) {
            newData[i] = this->data[i];
        }
        newData[strLength] = ch;
        newData[strLength+1] = T();

        delete[] data;
        data = newData;
        strLength += 1;

        return *this;
    }
    // Оператор конкатинації рядка до об'єкта
    
    MyString& operator+=(const MyString& other) { // додавання + присвоєння
        size_t newLength = this->strLength + other.strLength;
        T* newData = new T[newLength + 1];

        // Копіюємо перший рядок
        for (size_t i = 0; i < this->strLength; ++i) {
            newData[i] = this->data[i];
        }

        // Копіюємо другий рядок
        for (size_t i = 0; i < other.strLength; ++i) {
            newData[this->strLength + i] = other.data[i];
        }
        newData[newLength] = T(); // Додаємо нульовий термінатор
        delete[] this->data;
        this->strLength = newLength;
        this->data = newData;
        newData = nullptr;
        return *this;
    }
    
    // Оператор конкатинації рядка(Такого ж об'єкту)
    MyString operator+(const MyString& other) const {
        size_t newLength = this->strLength + other.strLength;
        T* newData = new T[newLength + 1];

        // Копіюємо перший рядок
        for (size_t i = 0; i < this->strLength; ++i) {
            newData[i] = this->data[i];
        }

        // Копіюємо другий рядок
        for (size_t i = 0; i < other.strLength; ++i) {
            newData[this->strLength + i] = other.data[i];
        }
        newData[newLength] = T(); // Додаємо нульовий термінатор

        MyString result;
        result.strLength = newLength;
        result.data = newData;
        newData = nullptr;
        return result;
    }
    // Оператор присвоєння "сивол + об'єкт"
    template<typename Y>
    friend MyString<Y> operator+(Y ch, const MyString<Y>& other);

    // Оператор додаванням "символа + об'єкт" із поверненням об'єкта
    MyString operator+(T ch) const {
        MyString res;
        res.strLength = this->strLength + 1;
        T* newData = new T[res.strLength + 1]; // +1 для нульового символу

        // Копіюємо існуючі символи
        for (int i = 0; i < this->strLength; i++) {
            newData[i] = this->data[i];
        }
        newData[strLength] = ch;
        newData[res.strLength] = T();
        res.data = newData;
        return res;
    }
    // Оператор порівняння рядків ==
    bool operator==(const MyString& other) const {
        if (this->strLength != other.strLength)
            return false;

        for (int i = 0; i < strLength; ++i) {
            if (this->data[i] != other.data[i])
                return false;
        }
        return true;
    }
    // Оператор порівняння рядків <
    bool operator<(const MyString& other) const {
        int minLen = (this->strLength < other.strLength) ? this->strLength : other.strLength;

        for (int i = 0; i < minLen; ++i) {
            if (this->data[i] < other.data[i])
                return true;
            else if (this->data[i] > other.data[i])
                return false;
        }

        return this->strLength < other.strLength;
    }
    // Оператор порівнянн !=
    bool operator!=(const MyString& other) const {
        return !(*this == other);
    }
    // Оператор порівнянн >
    bool operator>(const MyString& other) const {
        return other < *this;
    }
    // Оператор порівняння <=
    bool operator<=(const MyString& other) const {
        return !(*this > other);
    }
    // Операто порівняння >=
    bool operator>=(const MyString& other) const {
        return !(*this < other);
    }
    // Оператор множення рядка 
    MyString operator*(int count) const {
        MyString res;

        if (count <= 0) {
            return res;
        }

        res.strLength = this->strLength * count;
        T* newData = new T[res.strLength + 1];

        for (int i = 0; i < count; ++i) {
            for (int j = 0; j < this->strLength; ++j) {
                newData[i * this->strLength + j] = this->data[j];
            }
        }

        newData[res.strLength] = T();
        res.data = newData;
        return res;
    }

    friend istream& operator>>(istream& is, MyString<char>& str);


    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // Отримання довжини рядка
    size_t length() const {
        return strLength; // Використовуємо нову назву
    }

    // Перевірка чи пустий рядок
    bool isEmpty() const {
        return strLength == 0;
    }

    // Обнулення рядка
    void clear() {
        delete[] data;
        data = new T[1];
        data[0] = T();
        strLength = 0;
    }

    // Копіювання підрядка
    MyString substr(int pos, int len) const {
        if (pos < 0 || pos > strLength || pos + len > strLength || len < 0) {
            throw std::out_of_range("Invalid substring parameters");
        }
        else if (len == 0 || pos == strLength) {
            return MyString();
        }

        MyString sub;
        sub.strLength = len;
        sub.data = new T[len + 1];
        for (int i = 0; i < len; ++i) { // Явно вказано int
            sub.data[i] = this->data[pos + i];
        }
        sub.data[len] = T();
        return sub;
    }



    // Прототип дружнього Перевантаження оператора виводу
    template <typename U>
    friend ostream& operator<<(ostream& os, const MyString<U>& str);

    // Метод для приміненія "налаштування" нащадка класу Transform до рядки цього об'єкта (ЗМІНА ОБЄКТУ) Динамічний
    void apply(const Transformer<T>& transformer) {
        for (int i = 0; i < strLength; ++i) {
            data[i] = transformer(data[i]);
        }
    }
    // Метод для приміненія "налаштування" нащадка класу Transform  (СТВОРЕННЯ НОВОГО ОБЄКТУ) Динамічний
    MyString<T> mapped(const Transformer<T>& transformer) const {
        MyString<T> result;
        result.strLength = this->strLength;
        result.data = new T[result.strLength + 1];

        for (int i = 0; i < result.strLength; ++i) {
            result.data[i] = transformer(this->data[i]);
        }

        result.data[result.strLength] = T(); // нульовий символ
        return result;
    }
    // Метод для приміненія "налаштування" нащадка класу Transform  (СТВОРЕННЯ НОВОГО ОБЄКТУ) Статичний
    template <typename Transformer>
    MyString<T> transform(const Transformer& transformer) const {
        MyString<T> result;
        result.strLength = strLength;
        result.data = new T[result.strLength + 1];

        for (int i = 0; i < strLength; ++i) {
            result.data[i] = transformer(data[i]);
        }

        result.data[result.strLength] = T();
        return result;
    }
    // Метод для приміненія "налаштування" нащадка класу Transform  (ЗМІНА ОБЄКТУ) Статичний
    template <typename Transformer>
    void modify(const Transformer& transformer) {
        for (int i = 0; i < strLength; ++i) {
            data[i] = transformer(data[i]);
        }
    }



    ~MyString() {
        delete[] data;
    }
};
template <typename U>
ostream& operator<<(ostream& os, const MyString<U>& str) {
    for (int i = 0; i < str.length(); ++i) {
        os << str.data[i];
    }
    return os;
}
// Оператор присвоєння "сивол + об'єкт"
template<typename Y>
MyString<Y> operator+(Y ch, const MyString<Y>& other) {
    MyString<Y> res;
    res.strLength = other.strLength + 1;
    Y* newData = new Y[res.strLength + 1];
    newData[0] = ch;
    for (int i = 1; i < res.strLength; i++) {
        newData[i] = other.data[i - 1];
    }
    newData[res.strLength] = Y();
    res.data = newData;
    return res;
}

// ">>" MyString<>
istream& operator>>(istream& is, MyString<char>& str) {
    char buffer[1024]; // тимчасовий буфер

    is >> buffer; // зчитує слово (до пробілу)

    delete[] str.data; // звільняємо старі дані
    str.strLength = 0;
    while (buffer[str.strLength] != '\0') {
        str.strLength++;
    }

    str.data = new char[str.strLength + 1];
    for (int i = 0; i < str.strLength; ++i) {
        str.data[i] = buffer[i];
    }
    str.data[str.strLength] = '\0';

    return is;
}

// Метод створення рядка з двох показчиків
template <typename K>
MyString<K> make_stringFromTwoPointers(const K* begin, const K* end) {
    return MyString<K>(begin, end);
}

int main() {
    MyString<char> str = "Example";
    MyString<char> B = "anotherString";
    cout << "\n\tHello! I writed my own string class \"MyString\". This is interface to check all functions";
    cout << "\n\tSo, to continue enter string for test: ";
    cin >> str;
    cout << "\n\n";
    cout << "\tOperators: " << "\n\tYourString \"+\" anotherString = " << str + B;
    cout << "\n\tYourString \"+\" anotherSymbol = " << str + 'C';
    cout << "\n\tAnotherSymbol \"+\" yourString = " << 'C' + str;
    str += "anotherString";
    cout << "\n\tYour string \"+=\" anotherString = " << str;
    str += 'C';
    cout << "\n\tYourString \"+=\" anotherSymbol = " << str;
    MyString<char> ab = "ab", aa = "a";
    bool a = ab > aa;
    cout << "\n\t ab \">\" a = " << a;
    a = ab < aa;
    cout << "\n\t ab \"<\" a = " << a;
    a = ab == aa;
    cout << "\n\t ab \"==\" a = " << a;
    a = ab <= aa;
    cout << "\n\t ab \"<=\" a = " << a;
    a = ab >= aa;
    cout << "\n\t ab \">=\" a = " << a;
    a = ab != aa;
    cout << "\n\t ab \"!=\" a = " << a;
    cout << "\n\tYourStr * 3 = " << str * 3;
    //
    cout << "\n\tMethods: \n\tYourString.length() = " << str.length() << "(" << str << ")";
    cout << "\n\tYourStr.IsEmpty() = " << str.isEmpty() << "\n\tYourStr.clear() = ";
    str.clear();
    cout << str << "\n\tYourStr.IsEmpty() = " << str.isEmpty();
    str = "ExampleTwo";
    cout << "\n\tNow yourStr =  " << str;
    ToStar s;
    ToUpper l;
    cout << "\n\tYourStr.mapped(ToStar) = " << str.mapped(s) << " and YourStr.transform(ToLower) = " << str.transform(l);
    cout << "\n\tYourStr.substr(0, 4) = " << str.substr(0, 4);
    return 0;
}