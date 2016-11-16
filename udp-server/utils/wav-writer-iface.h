#ifndef WAVWRITERIFACE_H
#define WAVWRITERIFACE_H

namespace iz {

/// typical wav interface
/// \brief The WavIface class
///
class WavIface
{
public:
    /// opens a wav file
    /// \brief open
    /// \param perms - RW perms
    /// \return true on success, false else
    ///
    virtual bool open(const char* perms) = 0;

    /// closes the file
    /// \brief close
    ///
    virtual void close() = 0;

    /// write N samples of data to file
    /// \brief write
    /// \param data - samples pointer
    /// \param len - num samples
    /// \return samples written
    ///
    virtual int write(short int data[], int len) = 0;

    /// setup a wav header
    /// \brief setupWave
    /// \param samples_per_sec
    /// \param bits_per_sec
    /// \param riff_len
    /// \param fmt_len
    /// \param audio_fmt
    /// \param chann_cnt
    ///
    virtual void setupWave(int samples_per_sec,
                           int bits_per_sec,
                           int riff_len,
                           int fmt_len,
                           short audio_fmt,
                           short chann_cnt) = 0;

    /// read data from file - not used anywhere
    /// \brief read
    /// \return data from the file
    ///
    virtual void* read() = 0;

    /// return OK if file is opened
    /// \brief isOpened
    /// \return true if opened, otherwise false
    ///
    virtual bool isOpened() const = 0;

};

} // iz

#endif // WAVWRITERIFACE_H
