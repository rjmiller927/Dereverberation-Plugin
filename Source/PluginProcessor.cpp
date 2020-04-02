/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/



#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DereverbAudioProcessor::DereverbAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
        // ======================================================
        // Initialize FFT order in the constructor
        // ======================================================
        fft(fftOrder),
        window(fftSize, dsp::WindowingFunction<float>::hann) // Initialize window function
#endif
{
    
    dereverbFilter = new Dereverb;
    
}

DereverbAudioProcessor::~DereverbAudioProcessor()
{
    delete dereverbFilter;
}

//==============================================================================
const String DereverbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DereverbAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DereverbAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DereverbAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DereverbAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DereverbAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DereverbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DereverbAudioProcessor::setCurrentProgram (int index)
{
}

const String DereverbAudioProcessor::getProgramName (int index)
{
    return {};
}

void DereverbAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DereverbAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    Fs = sampleRate;
    N = samplesPerBlock;
    
    
    // Initialize FFT settings
    
}

void DereverbAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DereverbAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DereverbAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Use audio-buffer constructor of dsp::AudioBlock to use in JUCE DSP module
    // dsp::AudioBlock<float> block(buffer);
    
    // Cast buffer to an AudioSourceChannelInfo type
    AudioSourceChannelInfo bufferToFill = (AudioSourceChannelInfo(buffer));
    
    
    for (int channel = 0; channel < buffer.getNumChannels(); channel++){
        
        // Prepare next audio block for FFT
        getNextAudioBlock(bufferToFill, channel);
        
        if (nextFFTBlockReady == true){
            
            // Apply window
            window.multiplyWithWindowingTable(fftData[channel],sizeof(fftData[channel]));
            
            // FFT. According to JUCE documentation, the forward FFT interleaves real and imaginary parts, such that the first value is real and the second value is the associated imaginary component.
            fft.performRealOnlyForwardTransform(fftData[channel]);
            
            // Dereverb Processing
            // ....
            
            // IFFT
            fft.performRealOnlyInverseTransform(fftData[channel]);
            
            // Reset nextFFTBlockReady
            nextFFTBlockReady = false;
            
        }
        
    }
    
    
    // Replaces our block with a new one that has the gain that has been changed
    // block.copyTo(buffer);
    
    
}

void DereverbAudioProcessor::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill, int channel){
    if (bufferToFill.buffer->getNumChannels() > 0){
        auto *channelData = bufferToFill.buffer->getReadPointer(channel, bufferToFill.startSample);
        
        for (auto i=0; i < bufferToFill.numSamples; i++){
            pushNextSampleIntoFifo(channelData[i], channel);
        }
    }
}


void DereverbAudioProcessor::pushNextSampleIntoFifo(float sample, int channel) noexcept{
    if (fifoIndex[channel] == fftSize){
        if (!nextFFTBlockReady){
            zeromem(fftData, sizeof(fftData));
            memcpy(fftData, fifo, sizeof(fifo));
            nextFFTBlockReady = true;
        }
        fifoIndex[channel] = 0;
    }
    
    fifo[channel][fifoIndex[channel]] = sample;
    fifoIndex[channel]++;
    
}

//==============================================================================
bool DereverbAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DereverbAudioProcessor::createEditor()
{
    return new DereverbAudioProcessorEditor (*this);
}

//==============================================================================
void DereverbAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DereverbAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DereverbAudioProcessor();
}




