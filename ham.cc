#include "ham.h"

using namespace std;
using namespace ham;

const string usage = "usage: ham <questions.txt>";

int main (int argc, char **argv)
{
    try
    {
        if (argc != 2)
            throw runtime_error (usage);

        clog << "questions " << argv[1] << endl;

        auto subelements = parse_questions (argv[1]);

        while (1)
        {
            for (size_t i = 0; i < subelements.size (); ++i)
                clog << i << ' ' << subelements[i].description << endl;
            clog << "A All" << endl;
            clog << "R Random" << endl;
            clog << "Q Quit" << endl;
            clog << "Choose a topic: " << endl;
            char ch;
            cin >> ch;
            unsigned index;

            clog << ch << endl;
            switch (ch)
            {
                case 'a':
                case 'A':
                index = ~0;
                break;
                case 'r':
                case 'R':
                index = rand () % subelements.size ();
                break;
                default:
                index = ch - 0x30;
                break;
            }

            clog << "index " << index << endl;

            if (index > subelements.size ())
            {
                clog << "Invalid selection" << endl;
                break;
            }

            clog << subelements[index].description << endl;

            for (auto j : subelements[index].groups)
            {
                clog << j.name << endl;
                clog << j.description << endl;

                for (auto k : j.questions)
                {
                    clog << "subelement_number " << k.subelement_number << endl;
                    clog << "group_number " << k.group_number << endl;
                    clog << "rule " << k.rule << endl;
                    clog << "question " << k.q << endl;
                    clog << "answer " << k.answer << endl;
                    for (auto l : k.answers)
                        clog << '\t' << l << endl;
                }
            }
        }

        return 0;
    }
    catch (const exception &e)
    {
        cerr << e.what () << endl;
        return -1;
    }
}
