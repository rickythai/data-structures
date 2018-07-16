#include "String.hpp"
#include "OutOfBoundsException.hpp"

// Private member functions

// Private member function that comapres two array of chars to a
// certain length and returns a zero if they are equal and a
// non-zero number if they are not.
int String::compare(const char* left, const char* right, int len) const
{
    for (int i = 0; i < len; i++)
        if (left[i] != right[i])
            return left[i] - right[i];
    return 0;
}

// Private member function that returns the address of where the
// characters match.
char* String::match(char* str, int c) const
{
    for (int i = 0; str[i] != '\0'; i++)
        if (str[i] == c)
            return &str[i];
    return nullptr;
}

// Private member function that copies each char into an array of char
// for this string.
void String::copy(char* dest, const char* src) const
{
    int i;
    for (i = 0; src[i] != '\0'; i++)
        dest[i] = src[i];
    dest[i] = '\0';
}

// Private member function that returns number of char in C-string
int String::size(const char* chars) const
{
    int counter = 0;
    for (int i = 0; chars[i] != '\0'; i++)
        counter++;
    return counter + 1;
}

// Constructors
String::String()
{
    buf = new char[1];
    buf[0] = '\0';
}

String::String(const char* chars)
{
    buf = new char[size(chars)];
    copy(buf, chars);
}

String::String(const String& s)
{
    buf = new char[size(s.buf)];
    copy(buf, s.buf);
}

// Destructor
String::~String() noexcept
{
    delete [] buf;
}

// Assignment operator that sets this string equal to another
String& String::operator=(const String& s)
{ 
    char* temp = new char[size(s.buf)];

    copy(temp, s.buf);

    delete [] buf;
    buf = temp;
    return *this;
}

// Modifies this string by adding on another string to it
void String::append(const String& s)
{
   char* temp = new char[size(buf) + size(s.buf) - 1];
   int current = size(buf)-1;

   copy(temp, buf);
   copy((temp + current), s.buf);

   delete [] buf;
   buf = temp;
}

// Returns one of the characters in the string given an index
char String::at(unsigned int index) const
{
    if (index >= size(buf) - 1)
        throw OutOfBoundsException();
    return buf[index];
}
char& String::at(unsigned int index)
{
    if (index >= size(buf) - 1)
        throw OutOfBoundsException();
    return buf[index];
}

// Makes the string empty
void String::clear()
{
    delete [] buf;
    buf = new char[1];
    buf[0] = '\0';
}

// Compares the contents of this string to another string
int String::compareTo(const String& s) const noexcept
{
    for (int i = 0; buf[i] != '\0' || s.buf[i] != '\0'; i++)
        if (buf[i] != s.buf[i])
            return buf[i] - s.buf[i];
    return 0;
}

// Returns a string of this string followed by another string
String String::concatenate(const String& s) const
{
    char* temp = new char[size(buf)+size(s.buf) - 1];
    int current = size(buf) - 1;

    copy(temp, buf);
    copy((temp + current), s.buf);
    String newString(temp);
    delete [] temp;

    return newString;
}

// Returns true if this string contains the given substring
bool String::contains(const String& substring) const noexcept
{
    char *base = buf;
    char *str;
    int len = size(substring.buf) - 1;

    while (*base != '\0' && *substring.buf != '\0')
    {
        str = match(base, substring.buf[0]);

        if (str == nullptr)
            return false;

        if (compare(str, substring.buf, len) == 0)
            return true;
        else
            base = str + 1;
    }
    
    if (*substring.buf == '\0')
        return true;
    return false;
}

// Returns true if this string is equivalent to another string
bool String::equals(const String& s) const noexcept
{
    return this->compareTo(s) == 0;
}

// Returns the index where the given substring is found within this string
int String::find(const String& substring) const noexcept
{
    char *base = buf;
    char *str;
    int len = size(substring.buf) - 1;

    while (*base != '\0' && *substring.buf != '\0')
    {
        str = match(base, substring.buf[0]);
        if (str == nullptr)
            return -1;

        if (compare(str, substring.buf, len) == 0)
            return str - buf;
        else
            base = str + 1;
    }
    if (*base == '\0')
        return -1;
    return 0;
}

// Returns true if this string is empty
bool String::isEmpty() const noexcept
{
    return size(buf) <= 1;
}

// Returns the number of characters in this string
unsigned int String::length() const noexcept
{
    return size(buf) - 1;
}

// Returns a substring of this string
String String::substring(unsigned int startIndex, unsigned int endIndex) const
{
    if (startIndex > size(buf) || endIndex > size(buf))
        throw OutOfBoundsException();
    else if (startIndex >= endIndex)
        return String();

    char* temp = new char[endIndex-startIndex + 1];

    int index = 0;
    for (int i = startIndex; i < endIndex; i++)
    {
        temp[index] = buf[i];
        index++;
    }

    temp[index] = '\0';
    String newString(temp);
    delete [] temp;

    return newString;
}

// Returns a C-style string equivalent to this string
const char* String::toChars() const noexcept
{
    return buf;
}

