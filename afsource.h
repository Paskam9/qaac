#ifndef AFSOURCE_H
#define AFSOURCE_H

#include "AudioFileX.h"
#include "ExtAudioFileX.h"
#include "channel.h"

x::shared_ptr<ISource>
AudioFileOpenFactory(InputStream &stream, const std::wstring &path);

class AFSource: public ISource, public ITagParser,
    public PartialSource<AFSource>
{
    AudioFileX m_af;
    uint64_t m_offset;
    x::shared_ptr<InputStream> m_stream;
    std::vector<uint32_t> m_chanmap;
    std::map<uint32_t, std::wstring> m_tags;
    SampleFormat m_format;
public:
    AFSource(AudioFileX &af, x::shared_ptr<InputStream> &stream);
    /*
     * XXX: AudioFile_GetSizeProc is called inside of AudioFileClose().
     * Therefore, we must first call AudioFileClose() before destruction.
     */
    ~AFSource() { m_af.attach(0, false); }
    uint64_t length() const { return getDuration(); }
    const SampleFormat &getSampleFormat() const { return m_format; }
    const std::vector<uint32_t> *getChannelMap() const
    {
	return m_chanmap.size() ? &m_chanmap: 0;
    }
    size_t readSamples(void *buffer, size_t nsamples);
    void skipSamples(int64_t count) { m_offset += count; }
    const std::map<uint32_t, std::wstring> &getTags() const { return m_tags; }
    const std::vector<std::pair<std::wstring, int64_t> >
	*getChapters() const { return 0; }
};

class ExtAFSource: public ISource, public ITagParser,
    public PartialSource<ExtAFSource>
{
    AudioFileX m_af;
    ExtAudioFileX m_eaf;
    uint64_t m_offset;
    x::shared_ptr<InputStream> m_stream;
    std::vector<uint32_t> m_chanmap;
    std::map<uint32_t, std::wstring> m_tags;
    SampleFormat m_format;
public:
    ExtAFSource(AudioFileX &af, x::shared_ptr<InputStream> &stream,
		const std::wstring &path);
    ~ExtAFSource()
    {
	m_af.attach(0, false);
	m_eaf.attach(0, false);
    }
    uint64_t length() const { return getDuration(); }
    const SampleFormat &getSampleFormat() const { return m_format; }
    const std::vector<uint32_t> *getChannelMap() const
    {
	return m_chanmap.size() ? &m_chanmap: 0;
    }
    size_t readSamples(void *buffer, size_t nsamples);
    void skipSamples(int64_t count);
    const std::map<uint32_t, std::wstring> &getTags() const { return m_tags; }
    const std::vector<std::pair<std::wstring, int64_t> >
	*getChapters() const { return 0; }
};
#endif
