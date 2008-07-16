//
// C++ Interface: filterParameter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
//
#ifndef __FILTERPARAMETER_H__
#define __FILTERPARAMETER_H__

#include <vector>
#include <string>
#include <typeinfo>

#define CheckParameterTypeMacro(a,b) abstractParameter::checkType<a>(b)

namespace filter
{

/** *************************************************************
abstractParameter
*****************************************************************/

/**
    Classe astratta per contenere un parametro

    @author Daniele Domenichelli
*/
class abstractParameter
{
public:
    abstractParameter(std::string name) : m_Name(name) {}
    ~abstractParameter() {}

    inline void setName(const std::string name) {m_Name = name;}
    inline const std::string getName(void) {return m_Name;}

    virtual const char* getType(void) = 0;
    virtual void Update(void) = 0;


    /**
        Esegue un confronto tra il tipo della classe T e il tipo del parametro contenuto in par

        Può essere usato direttamente con "abstractParameter::checkType\<T\>,(par)" o con la macro "CheckTypeMacro(T,par)"
    */
    template <class T>
    inline static bool checkType(abstractParameter* par){return (typeid(T).name() == par->getType()) ? true : false;}

protected:
    std::string m_Name;
};


/** *************************************************************
filterParameter
*****************************************************************/

/**
    Template per definire un parametro di tipo T

    @author Daniele Domenichelli
*/
template <class T> class filterParameter : public abstractParameter
{
public:
    filterParameter(T& ref, std::string name, T defaultValue) :
                    abstractParameter(name),
                    m_Ref(ref),
                    m_DefaultValue(defaultValue),
                    m_Value(defaultValue),
                    m_HasMax(false),
                    m_HasMin(false){}
    filterParameter(T& ref, std::string name, T defaultValue, T min, T max) :
                    abstractParameter(name),
                    m_Ref(ref),
                    m_DefaultValue(defaultValue),
                    m_Value(defaultValue),
                    m_HasMax(true),
                    m_HasMin(true),
                    m_MaxValue(max),
                    m_MinValue(min){}
    ~filterParameter() {}

    /**
        Funzioni set e get per il valore
    */
    inline T getValue(void) {return m_Value;}
    void setValue(T value)
    {
        if (m_HasMin && m_HasMax)
        {
            T tmp = (value < m_MinValue) ? m_MinValue : value;
            m_Value = (tmp > m_MaxValue) ? m_MaxValue : tmp;
        }
        else if (m_HasMin)
            m_Value = (value < m_MinValue) ? m_MinValue : value;
        else if (m_HasMax)
            m_Value = (value > m_MaxValue) ? m_MaxValue : value;
        else
            m_Value = value;
    }

    /**
        Funzioni set e get per il valore di default.
        Nota: Non può essere settato un valore diverso da quello impostato alla creazione
    */
    inline T getDefaultValue(void) {return m_DefaultValue;}
    inline void setDefaultValue(void) {m_Value = m_DefaultValue;}


    /**
        Funzioni per il minimo e  il massimo
        Nota: Utilizzare sempre hasMin o hasMax prima di usare le funzioni get perchè non viene
              eseguito alcuna inizializzazione nè alcun controllo, di conseguenza l'operazione
              potrebbe avere esiti inaspettati
    */
    inline bool hasMax(void) {return m_HasMax;}
    inline bool hasMin(void) {return m_HasMin;}
    inline T getMaxValue(void) {return m_MaxValue;}
    inline T getMinValue(void) {return m_MinValue;}
    inline void setMaxValue(T value) {m_MaxValue = value; m_HasMax=true;}
    inline void setMinValue(T value) {m_MinValue = value; m_HasMin=true;}

    /**
        Restituisce il nome del typeid del parametro.
        Per eseguire un confronto di tipo è molto più conveniente utilizzare la macro "CheckTypeMacro(T,par)"
    */
    const char* getType(void) {return typeid(T).name();}

    inline void Update(void) {m_Ref = m_Value;}

private:
    T& m_Ref;
    T m_DefaultValue;
    T m_Value;
    bool m_HasMax, m_HasMin;
    T m_MaxValue, m_MinValue;

};


/** *************************************************************
filterParameterList
*****************************************************************/


class filterParameterList
{
public:
    filterParameterList()
    {
        m_ParameterList = new ParameterList;
    }
    ~filterParameterList()
    {
        ParameterList::iterator iter;
        for(iter = m_ParameterList->begin(); iter < m_ParameterList->end(); iter++)
            delete *iter;
        delete m_ParameterList;
    }


    /**
        Aggiunge un parametro
        Restituisce true se è stato aggiunto correttamente, false in caso contrario
    */
    bool addParameter(abstractParameter* par)
    {
        ParameterList::iterator iter;
        for(iter = m_ParameterList->begin(); iter < m_ParameterList->end(); iter++)
            if (par->getName() == (*iter)->getName())
                return false;
        m_ParameterList->push_back(par);
        return true;
    }

    abstractParameter* getParameter(std::string name)
    {
        ParameterList::iterator iter;
        for(iter = m_ParameterList->begin(); iter < m_ParameterList->end(); iter++)
            if (name == (*iter)->getName())
                return *iter;
        return NULL;
    }

    abstractParameter* getParameter(unsigned int index)
    {
        if (index < m_ParameterList->size())
            return m_ParameterList->at(index);
        return NULL;
    }

    int getSize(void) {return m_ParameterList->size();}

    void Update(void)
    {
        ParameterList::iterator iter;
        for(iter = m_ParameterList->begin(); iter < m_ParameterList->end(); iter++)
            (*iter)->Update();
    }

private:
    typedef std::vector<abstractParameter*> ParameterList;
    ParameterList* m_ParameterList;
};



}; //namespace filter

#endif // __FILTERPARAMETER_H__
