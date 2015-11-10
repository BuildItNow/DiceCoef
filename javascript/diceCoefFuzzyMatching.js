(function()
{
    var BIGRAM_SIZE = 2;
    var HALF_BIGRAM_SIZE = BIGRAM_SIZE/2;
    var DiceCoefFuzzyMatching = function(pattern)
    {
        this.setPatternBigrams(pattern);
    }

    var pro = DiceCoefFuzzyMatching.prototype;

    pro.setPatternBigrams = function(pattern)
    {
        this._patternBigrams = [];
        this._calBigrams(this._patternBigrams, pattern);
    }

    pro.simiCoef = function(search)
    {
        if(!this._patternBigrams || this._patternBigrams.length === 0)
        {
            return 0;
        }

        this._searchBigrams = [];
        this._calBigrams(this._searchBigrams, search);
        
        if(this._searchBigrams.length === 0)
        {
            return 0;
        }

        var pbgs = this._patternBigrams;
        var sbgs = this._searchBigrams;
        var len  = pbgs.length+sbgs.length;
        var mat  = 0;
        var bigram = "";
        
        for(var i=0,i_sz=pbgs.length; i<i_sz; ++i)
        {
            bigram = pbgs[i];
            for(var j=0,j_sz=sbgs.length; j<j_sz; ++j)
            {
                if(bigram === sbgs[j])
                {
                    ++mat;
                    sbgs[j] = sbgs[j_sz-1];
                    sbgs.pop();
                    break;
                }
            }
        }

        return mat*2/len;
    }
    
    pro._calBigrams = function(bigrams, input)
    {
        if(!input)
        {
            return ;
        }

        bigrams.push(input.substring(0, HALF_BIGRAM_SIZE));
        if(input.length <= HALF_BIGRAM_SIZE)
        {
            return ;
        }

        var pos = 0;
        var end = input.length;
        for(; pos<end; pos+=HALF_BIGRAM_SIZE)    
        {
            bigrams.push(input.substring(pos, pos+BIGRAM_SIZE));
        }
    }

    var s_instance = undefined;
    DiceCoefFuzzyMatching.simiCoef = function(input0, input1)
    {
        if(!s_instance)
        {
            s_instance = new DiceCoefFuzzyMatching();
        }

        s_instance.setPatternBigrams(input0);
        return s_instance.simiCoef(input1);
    }
    
    if(typeof define === "function" && define.amd)
    {
	    define([], function()
        {
		    return  DiceCoefFuzzyMatching;
	    });
    }
    else if(window)
    {
        window.DiceCoefFuzzyMatching = DiceCoefFuzzyMatching; 
    }
    else if(module)
    {
        module.exports = DiceCoefFuzzyMatching; 
    }
})();


