//
// C++ Interface: filterInput
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __FILTERINPUT__
#define __FILTERINPUT__

#include <vector>
#include <string>
#include <typeinfo>

#define CheckInputTypeMacro(a,b) abstractInput::checkType<a>(b)

namespace filter
{

/** *************************************************************
abstractInput
*****************************************************************/
class abstractInput
{
public:
    abstractInput(std::string name) : m_Name(name) {}
    ~abstractInput() {}

    inline void setName(const std::string name) {m_Name = name;}
    inline const std::string getName(void) {return m_Name;}

    virtual const char* getType(void) = 0;
    virtual void Update(void) = 0;


    /**
        Esegue un confronto tra il tipo della classe T e il tipo del parametro contenuto in par

        Può essere usato direttamente con "abstractInput::checkType\<T\>,(par)" o con la macro "CheckTypeMacro(T,par)"
    */
    template <class T>
    inline static bool checkType(abstractInput* input){return (typeid(T).name() == input->getType()) ? true : false;}

protected:
    std::string m_Name;
};

/** *************************************************************
filterInput
*****************************************************************/
template <class T> class filterInput : public abstractInput
{
public:
    enum filterMode
    {
        fileMode = 0,
        vtkMode
    };
    filterInput(T& ref, std::string name, filterMode mode = fileMode) :  abstractInput(name),
                                                                         m_Mode(mode),
                                                                         m_Ref(ref) {}
    ~filterInput() {}
    inline const char* getType(void) {return typeid(T).name();}
    inline filterMode getMode(void) {return m_Mode;}
    inline void Update(void) {/*m_Ref->Update();*/}

    inline void Set(T ptr) {m_Ref = ptr;}
    inline T Get(void) {return m_Ref;}

private:
    filterMode m_Mode;
    T& m_Ref;
};

/** *************************************************************
filterInputList
*****************************************************************/


class filterInputList
{
typedef std::vector<abstractInput*> InputList;

public:

    filterInputList()
    {
        m_InputList = new InputList;
    }

    ~filterInputList()
    {
        InputList::iterator iter;
        for(iter = m_InputList->begin(); iter < m_InputList->end(); iter++)
            delete *iter;
        delete m_InputList;
    }

    /**
        Aggiunge un input
        Restituisce true se è stato aggiunto correttamente, false in caso contrario
    */
    bool addInput(abstractInput* input)
    {
        InputList::iterator iter;
        for(iter = m_InputList->begin(); iter < m_InputList->end(); iter++)
        if (input->getName() == (*iter)->getName())
            return false;
        m_InputList->push_back(input);
        return true;
    }

    abstractInput* getInput(std::string name)
    {
        InputList::iterator iter;
        for(iter = m_InputList->begin(); iter < m_InputList->end(); iter++)
            if (name == (*iter)->getName())
                return *iter;
        return NULL;
    }

    abstractInput* getInput(unsigned int index)
    {
        if (index < m_InputList->size())
            return m_InputList->at(index);
        return NULL;
    }

    void Update(void)
    {
        InputList::iterator iter;
        for(iter = m_InputList->begin(); iter < m_InputList->end(); iter++)
            (*iter)->Update();
    }

    int getSize(void) {return m_InputList->size();}

private:
    InputList* m_InputList;
};



} //namespace filter

#endif // __FILTERINPUT__
