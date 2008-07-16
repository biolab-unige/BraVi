//
// C++ Interface: filterOutput
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __FILTEROUTPUT__
#define __FILTEROUTPUT__

#include <vector>
#include <string>
#include <typeinfo>

#define CheckOutputTypeMacro(a,b) abstractOutput::checkType<a>(b)

namespace filter
{

/** *************************************************************
abstractOutput
*****************************************************************/
class abstractOutput
{
public:
    abstractOutput(std::string name) : m_Name(name) {}
    ~abstractOutput() {}

    inline void setName(const std::string name) {m_Name = name;}
    inline const std::string getName(void) {return m_Name;}

    virtual const char* getType(void) = 0;
    virtual void Update(void) = 0;


    /**
        Esegue un confronto tra il tipo della classe T e il tipo del parametro contenuto in par

        Può essere usato direttamente con "abstractOutput::checkType\<T\>,(par)" o con la macro "CheckTypeMacro(T,par)"
    */
    template <class T>
    inline static bool checkType(abstractOutput* output){return (typeid(T).name() == output->getType()) ? true : false;}

protected:
    std::string m_Name;
};

/** *************************************************************
filterOutput
*****************************************************************/
template <class T> class filterOutput : public abstractOutput
{
public:
    enum filterMode
    {
        fileMode = 0,
        vtkMode
    };
    filterOutput(T& ref, std::string name, filterMode mode = fileMode) : abstractOutput(name),
                                                                         m_Ref(ref),
                                                                         m_Mode(mode) {}
    ~filterOutput() {}
    inline const char* getType(void) {return typeid(T).name();}
    inline filterMode getMode(void) {return m_Mode;}
    inline void Update(void) {/*m_Ref->Update()*/}

    inline void Set(T ptr) {m_Ref = ptr;}
    inline T Get(void) {return m_Ref;}

private:
    T& m_Ref;
    filterMode m_Mode;
};


/** *************************************************************
filterOutputList
*****************************************************************/


class filterOutputList
{
typedef std::vector<abstractOutput*> OutputList;

public:

    filterOutputList()
    {
        m_OutputList = new OutputList;
    }

    ~filterOutputList()
    {
        OutputList::iterator iter;
        for(iter = m_OutputList->begin(); iter < m_OutputList->end(); iter++)
            delete *iter;
        delete m_OutputList;
    }

    /**
        Aggiunge un output
        Restituisce true se è stato aggiunto correttamente, false in caso contrario
    */
    bool addOutput(abstractOutput* output)
    {
        OutputList::iterator iter;
        for(iter = m_OutputList->begin(); iter < m_OutputList->end(); iter++)
        if (output->getName() == (*iter)->getName())
            return false;
        m_OutputList->push_back(output);
        return true;
    }

    abstractOutput* getOutput(std::string name)
    {
        OutputList::iterator iter;
        for(iter = m_OutputList->begin(); iter < m_OutputList->end(); iter++)
            if (name == (*iter)->getName())
                return *iter;
        return NULL;
    }

    abstractOutput* getOutput(unsigned int index)
    {
        if (index < m_OutputList->size())
            return m_OutputList->at(index);
        return NULL;
    }

    void Update(void)
    {
        OutputList::iterator iter;
        for(iter = m_OutputList->begin(); iter < m_OutputList->end(); iter++)
            (*iter)->Update();
    }

    int getSize(void) {return m_OutputList->size();}

private:
    OutputList* m_OutputList;
};



} //namespace filter

#endif // __FILTEROUTPUT__
