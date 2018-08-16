/*************************************************************************************
 * Original code copyright (C) 2012 Steve Folta
 * Converted to Juce module (C) 2016 Leo Olivers
 * Forked from https://github.com/stevefolta/SFZero
 * For license info please see the LICENSE file distributed with this source code
 *************************************************************************************/
#ifndef SF2_H_INCLUDED
#define SF2_H_INCLUDED

#include "SF2WinTypes.h"
#include <vector>
//#define SF2Field(type, name) type name;

namespace sfzero
{
    
    namespace SF2
    {
        
        struct rangesType
        {
            byte lo, hi;
        };
        
        union genAmountType {
            rangesType range;
            short shortAmount;
            word wordAmount;
        };
        
        struct iver
        {
//#include "sf2-chunks/iver.h"
            word major;
            word minor;
            void readFrom(juce::InputStream *file);
        };
        
        struct phdr
        {
//#include "sf2-chunks/phdr.h"
            char20 presetName;
            word preset;
            word bank;
            word presetBagNdx;
            dword library;
            dword genre;
            dword morphology;
            void readFrom(juce::InputStream *file);
            
            static const int sizeInFile = 38;
        };
        
        struct pbag
        {
//#include "sf2-chunks/pbag.h"
            word genNdx;
            word modNdx;
            void readFrom(juce::InputStream *file);
            
            static const int sizeInFile = 4;
        };
        
        struct pmod
        {
//#include "sf2-chunks/pmod.h"
            word modSrcOper;
            word modDestOper;
            short modAmount;
            word modAmtSrcOper;
            word modTransOper;
            void readFrom(juce::InputStream *file);
            
            static const int sizeInFile = 10;
        };
        
        struct pgen
        {
//#include "sf2-chunks/pgen.h"
            word genOper;
            genAmountType genAmount;

            void readFrom(juce::InputStream *file);
            
            static const int sizeInFile = 4;
        };
        
        struct inst
        {
//#include "sf2-chunks/inst.h"
            char20 instName;
            word instBagNdx;
            void readFrom(juce::InputStream *file);
            
            static const int sizeInFile = 22;
        };
        
        struct ibag
        {
//#include "sf2-chunks/ibag.h"
            word instGenNdx;
            word instModNdx;

            void readFrom(juce::InputStream *file);
            
            static const int sizeInFile = 4;
        };
        
        struct imod
        {
//#include "sf2-chunks/imod.h"
            word modSrcOper;
            word modDestOper;
            short modAmount;
            word modAmtSrcOper;
            word modTransOper;

            void readFrom(juce::InputStream *file);
            
            static const int sizeInFile = 10;
        };
        
        struct igen
        {
//#include "sf2-chunks/igen.h"
            word genOper;
            genAmountType genAmount;
            void readFrom(juce::InputStream *file);
            
            static const int sizeInFile = 4;
        };
        
        struct shdr
        {
//#include "sf2-chunks/shdr.h"
            char20 sampleName;
            dword start;
            dword end;
            dword startLoop;
            dword endLoop;
            dword sampleRate;
            byte originalPitch;
            char pitchCorrection;
            word sampleLink;
            word sampleType;

            void readFrom(juce::InputStream *file);
            
            static const int sizeInFile = 46;
        };
        
        struct HydraData
        {
            virtual void readFrom(juce::InputStream *file) = 0;
            virtual ~HydraData() {}
            
            
        };
        
        struct Hydra
        {
//            phdr *phdrItems;
//            pbag *pbagItems;
//            pmod *pmodItems;
//            pgen *pgenItems;
//            inst *instItems;
//            ibag *ibagItems;
//            imod *imodItems;
//            igen *igenItems;
//            shdr *shdrItems;
            
            std::vector<phdr> phdrItems;
            std::vector<pbag>pbagItems;
            std::vector<pmod>pmodItems;
            std::vector<pgen>pgenItems;
            std::vector<inst>instItems;
            std::vector<ibag>ibagItems;
            std::vector<imod>imodItems;
            std::vector<igen>igenItems;
            std::vector<shdr>shdrItems;
            
            template<typename T>
            void readHelper(const sfzero::RIFFChunk& chunk,
                            std::vector<T>& chunkItems,
                            juce::InputStream* file)
            {
                int numItems = chunk.size / T::sizeInFile;
                for( int i = 0; i < numItems; ++i )
                {
                    T t;
                    t.readFrom(file);
                    chunkItems.push_back(t);
                }
            }
            
//            int phdrNumItems, pbagNumItems, pmodNumItems, pgenNumItems;
//            int instNumItems, ibagNumItems, imodNumItems, igenNumItems;
//            int shdrNumItems;
            
            Hydra();
            ~Hydra();
            
            void readFromOld(juce::InputStream *file, juce::int64 pdtaChunkEnd);
            void readFrom(juce::InputStream *file, juce::int64 pdtaChunkEnd);
            bool isComplete();
        };
    }
}

#undef SF2Field

#endif // SF2_H_INCLUDED
