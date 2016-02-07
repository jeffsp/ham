#ifndef HAM_H
#define HAM_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace ham
{

using namespace std;

struct question
{
    unsigned subelement_number;
    unsigned group_number;
    string rule;
    string q;
    int answer;
    vector<string> answers;
};

typedef vector<question> questions_t;

struct group
{
    string name;
    string description;
    questions_t questions;
};

typedef vector<group> groups_t;

struct subelement
{
    string name;
    string description;
    groups_t groups;
};

typedef vector<subelement> subelements_t;

subelements_t parse_questions (const string &s)
{
    subelements_t subelements;
    clog << "parsing " << s << endl;
    ifstream ifs (s.c_str ());
    if (!ifs)
        throw  runtime_error ("can't open file");
    while (ifs)
    {
        string s;
        getline (ifs, s);
        // ignore blank lines
        if (s.empty ())
            continue;
        // ignore question separators
        if (s[0] == '~')
            continue;
        stringstream ss (s);
        string tag;
        ss >> tag;
        // is it a subelement tag?
        if (tag == "SUBELEMENT")
        {
            subelement e;
            // get the name
            ss >> e.name;
            // get rid of leading whitespace
            ws (ss);
            // rest of the line is the description
            getline (ss, e.description);
            subelements.push_back (e);
        }
        // is it a group description?
        else if (s[0] == 'T' && s[3] == ' ')
        {
            if (subelements.empty ())
                throw runtime_error ("can't parse subelements");
            // set the name
            group g { tag };
            // get rid of leading whitespace
            ws (ss);
            // rest of the line is the description
            getline (ss, g.description);
            subelements.back ().groups.push_back (g);
        }
        // is it a question header?
        else if (s[0] == 'T' && s[5] == ' ' && s[6] == '(' && s[8] == ')')
        {
            if (subelements.empty ())
                throw runtime_error ("can't parse subelements");
            if (subelements.back ().groups.empty ())
                throw runtime_error ("can't parse subelements");
            question q;
            q.subelement_number = s[1] - 0x30;
            q.group_number = s[2] - 'A';
            if (s.size () > 10)
                q.rule = string (s.begin () + 10, s.end ());
            getline (ifs, q.q);
            q.answer = s[7] - 'A';
            subelements.back ().groups.back ().questions.push_back (q);
        }
        // is it an answer?
        else if (s[1] == '.')
        {
            if (subelements.empty ())
                throw runtime_error ("error parsing questions");
            if (subelements.back ().groups.empty ())
                throw runtime_error ("error parsing questions");
            if (subelements.back ().groups.back ().questions.empty ())
                throw runtime_error ("error parsing questions");
            if (subelements.back ().groups.back ().questions.back ().answers.size () == 4)
                throw runtime_error ("error parsing questions");
            subelements.back ().groups.back ().questions.back ().answers.push_back (s);
        }
        // what is it?
        else
            throw runtime_error ("error parsing questions: unrecognized line");
    }
    return subelements;
}

}

#endif
