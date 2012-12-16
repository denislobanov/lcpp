#include <iostream>

class talk
{
    public:
    talk(std::string name);
    void say(std::string text);
    
    private:
    void output(std::string text);
    
    std::string my_name;
    
};

talk::talk(std::string name)
{
    my_name = name;
}

void talk::say(std::string text)
{
    output(text);
}

void talk::output(std::string text)
{
    std::cout << my_name << ": " << text << std::endl;
}

int main()
{
    talk voice("overcomplicated");
    voice.say("hello world");
    
    return 0;
}

