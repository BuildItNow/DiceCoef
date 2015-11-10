#include <vector>
#include <string>
#include <cstring>

class CDiceCoefFuzzyMatching
{
    typedef short               Bigram;
    typedef std::vector<Bigram> Bigrams;
    enum
    {
        BIGRAM_SIZE      = sizeof(Bigram),
        HALF_BIGRAM_SIZE = BIGRAM_SIZE>>1,
        MAX_BIGRAM_COUNT = 512,
        MAX_BUFFER_SIZE  = MAX_BIGRAM_COUNT*BIGRAM_SIZE+BIGRAM_SIZE,
    };

public:
    CDiceCoefFuzzyMatching(const std::string& strSearchPattern = "")
    {
        SetPatternBigrams(strSearchPattern);
    }

    //! Calculate the simility
    float CalSimiCoef(const std::string& strMatch)
    {
        CalBigrams(m_beMatched, strMatch);

        if(m_searchPattern.empty() || m_beMatched.empty())
        {
            return 0.0f;
        }

        int nLen        = m_searchPattern.size()+m_beMatched.size();
        int nMatched    = 0;
        Bigram bigram   = Bigram(0);

        for(int i=0,i_sz=m_searchPattern.size(); i<i_sz; ++i)
        {
            bigram = m_searchPattern[i];
            for(int j=0,j_sz=m_beMatched.size(); j<j_sz; ++j)
            {
                if(bigram == m_beMatched[j])
                {
                    ++nMatched;
                    m_beMatched[j] = m_beMatched.back();
                    m_beMatched.resize(j_sz-1);
                    break;
                }
            }
        }

        return nMatched*2/(float)nLen;
    }
    
    void SetPatternBigrams(const std::string& strSearchPattern)
    {
        CalBigrams(m_searchPattern, strSearchPattern);
    }

protected:

    // Calculate the bigrams
    void CalBigrams(Bigrams& bigrams, const std::string& strInput)
    {
        bigrams.resize(0);

        if(strInput.empty())
        {
            return ;
        }

        int nSize = strInput.size();
        nSize    += BIGRAM_SIZE-1;
        nSize    &= ~(BIGRAM_SIZE-1);
        if(nSize/BIGRAM_SIZE > MAX_BIGRAM_COUNT)
        {
            return ;
        }

        unsigned char pData[MAX_BUFFER_SIZE];  
        *(Bigram*)(pData+nSize-BIGRAM_SIZE) = 0;
        *(Bigram*)(pData+nSize) = 0;

        memcpy(pData, strInput.c_str(), strInput.size());

        Bigram bigram = *(Bigram*)pData;
        memset((void*)(((unsigned char*)&bigram)+HALF_BIGRAM_SIZE), 0, HALF_BIGRAM_SIZE);
        bigrams.push_back(bigram);
        if(strInput.size()<=HALF_BIGRAM_SIZE) 
        {
            return ;
        }

        unsigned char* pos = pData;
        unsigned char* end = pData+nSize;
        for(; pos<end; pos += HALF_BIGRAM_SIZE)
        {
            bigrams.push_back(*(Bigram*)pos);
        }
    }
private:
    Bigrams     m_searchPattern;
    Bigrams     m_beMatched;

    // Static
public:
    static float SimiCoef(const std::string& strInput0, const std::string& strInput1)
    {
        static CDiceCoefFuzzyMatching s_instance;
        s_instance.SetPatternBigrams(strInput0);
        return s_instance.CalSimiCoef(strInput1);
    }
};

