/*************************************************************************************
 * Original code copyright (C) 2012 Steve Folta
 * Converted to Juce module (C) 2016 Leo Olivers
 * Forked from https://github.com/stevefolta/SFZero
 * For license info please see the LICENSE file distributed with this source code
 *************************************************************************************/
#include "SF2.h"
#include "RIFF.h"

#define readAbyte(name, file) name = (byte)file->readByte();
#define readAchar(name, file) name = file->readByte();
#define readAdword(name, file) name = (dword)file->readInt();
#define readAword(name, file) name = (word)file->readShort();
#define readAshort(name, file) name = file->readShort();
#define readAchar20(name, file) file->read(name, 20);
#define readAgenAmountType(name, file) name.shortAmount = file->readShort();

#define SF2Field(type, name) readA##type(name, file)

void sfzero::SF2::iver::readFrom(juce::InputStream *file)
{
    //SF2Field(word, major);
    major = (word)file->readShort();
    //SF2Field(word, minor);
    minor = (word)file->readShort();
}

void sfzero::SF2::phdr::readFrom(juce::InputStream *file)
{
//    SF2Field(char20, presetName)
    file->read(presetName, 20);
//    SF2Field(word, preset)
    preset = (word)file->readShort();
//    SF2Field(word, bank)
    bank = (word)file->readShort();
//    SF2Field(word, presetBagNdx)
    presetBagNdx = (word)file->readShort();
//    SF2Field(dword, library)
    library = (dword)file->readInt();
//    SF2Field(dword, genre)
    genre = (dword)file->readInt();
//    SF2Field(dword, morphology)
    morphology = (dword)file->readInt();
}

void sfzero::SF2::pbag::readFrom(juce::InputStream *file)
{
//    SF2Field(word, genNdx)
    genNdx = (word)file->readShort();
//    SF2Field(word, modNdx)
    modNdx = (word)file->readShort();
}

void sfzero::SF2::pmod::readFrom(juce::InputStream *file)
{
//    SF2Field(word, modSrcOper)
    modSrcOper = (word)file->readShort();
//    SF2Field(word, modDestOper)
    modDestOper = (word)file->readShort();
//    SF2Field(short, modAmount)
    modAmount = file->readShort();
//    SF2Field(word, modAmtSrcOper)
    modAmtSrcOper = (word)file->readShort();
//    SF2Field(word, modTransOper)
    modTransOper = (word)file->readShort();
}

void sfzero::SF2::pgen::readFrom(juce::InputStream *file)
{
//    SF2Field(word, genOper)
    genOper = (word)file->readShort();
//    SF2Field(genAmountType, genAmount)
    genAmount.shortAmount = file->readShort();
}

void sfzero::SF2::inst::readFrom(juce::InputStream *file)
{
//    SF2Field(char20, instName)
    file->read(instName, 20);
//    SF2Field(word, instBagNdx)
    instBagNdx = (word)file->readShort();
}

void sfzero::SF2::ibag::readFrom(juce::InputStream *file)
{
//    SF2Field(word, instGenNdx)
    instGenNdx = (word)file->readShort();
//    SF2Field(word, instModNdx)
    instModNdx = (word)file->readShort();
}

void sfzero::SF2::imod::readFrom(juce::InputStream *file)
{
//    SF2Field(word, modSrcOper)
    modSrcOper = (word)file->readShort();
//    SF2Field(word, modDestOper)
    modDestOper = (word)file->readShort();
//    SF2Field(short, modAmount)
    modAmount = file->readShort();
//    SF2Field(word, modAmtSrcOper)
    modAmtSrcOper = (word)file->readShort();
//    SF2Field(word, modTransOper)
    modTransOper = (word)file->readShort();
}

void sfzero::SF2::igen::readFrom(juce::InputStream *file)
{
//    SF2Field(word, genOper)
    genOper = (word)file->readShort();
//    SF2Field(genAmountType, genAmount)
    genAmount.shortAmount = file->readShort();
}

void sfzero::SF2::shdr::readFrom(juce::InputStream *file)
{
//    SF2Field(char20, sampleName)
    file->read(sampleName, 20);
//    SF2Field(dword, start)
    start = (dword)file->readInt();
//    SF2Field(dword, end)
    end = (dword)file->readInt();
//    SF2Field(dword, startLoop)
    startLoop = (dword)file->readInt();
//    SF2Field(dword, endLoop)
    endLoop = (dword)file->readInt();
//    SF2Field(dword, sampleRate)
    sampleRate = (dword)file->readInt();
//    SF2Field(byte, originalPitch)
    originalPitch = (byte)file->readByte();
//    SF2Field(char, pitchCorrection)
    pitchCorrection = file->readByte();
//    SF2Field(word, sampleLink)
    sampleLink = (word)file->readShort();
//    SF2Field(word, sampleType)
    sampleType = (word)file->readShort();
}

void sfzero::SF2::Hydra::readFrom(juce::InputStream* file, juce::int64 pdtaChunkEnd)
{
    auto check = [](sfzero::RIFFChunk& chunk, sfzero::fourcc chunkName)
    {
        sfzero::fourcc& chunkID = chunk.id;
        return
        chunkID[0] == chunkName[0] &&
        chunkID[1] == chunkName[1] &&
        chunkID[2] == chunkName[2] &&
        chunkID[3] == chunkName[3];
    };
    
    sfzero::fourcc phdrType = {'p','h','d','r'};
    sfzero::fourcc pbagType = {'p','b','a','g'};
    sfzero::fourcc pmodType = {'p','m','o','d'};
    sfzero::fourcc pgenType = {'p','g','e','n'};
    sfzero::fourcc instType = {'i','n','s','t'};
    sfzero::fourcc ibagType = {'i','b','a','g'};
    sfzero::fourcc imodType = {'i','m','o','d'};
    sfzero::fourcc igenType = {'i','g','e','n'};
    sfzero::fourcc shdrType = {'s','h','d','r'};
    
    while( file->getPosition() < pdtaChunkEnd )
    {
        sfzero::RIFFChunk chunk;
        chunk.readFrom(file);
        
        if( check(chunk, phdrType ) )
        {
            readHelper(chunk, phdrItems, file);
        }
        else if( check(chunk, pbagType ) )
        {
            readHelper(chunk, pbagItems, file);
        }
        else if( check(chunk, pmodType ) )
        {
            readHelper(chunk, pmodItems, file);
        }
        else if( check(chunk, pgenType ) )
        {
            readHelper(chunk, pgenItems, file);
        }
        else if( check(chunk, instType) )
        {
            readHelper(chunk, instItems, file);
        }
        else if( check(chunk, ibagType ) )
        {
            readHelper(chunk, ibagItems, file);
        }
        else if( check(chunk, imodType) )
        {
            readHelper(chunk, imodItems, file);
        }
        else if( check(chunk, igenType ) )
        {
            readHelper(chunk, igenItems, file);
        }
        else if( check(chunk, shdrType ) )
        {
            readHelper(chunk, shdrItems, file);
        }
        
        chunk.seekAfter(file);
    }
}

bool sfzero::SF2::Hydra::isComplete()
{
    return
    !phdrItems.empty() &&
    !pbagItems.empty() &&
    !pmodItems.empty() &&
    !pgenItems.empty() &&
    !instItems.empty() &&
    !ibagItems.empty() &&
    !imodItems.empty() &&
    !igenItems.empty() &&
    !shdrItems.empty();
}
