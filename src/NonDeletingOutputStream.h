#ifndef __NONDELETINGOUTPUTSTREAM_HEADER__
#define __NONDELETINGOUTPUTSTREAM_HEADER__

class NonDeletingOutputStream : public juce::OutputStream {
public:
  inline NonDeletingOutputStream(juce::OutputStream *const obj) noexcept : object(obj) {}

  inline ~NonDeletingOutputStream() {} // does not delete the object

  void flush() {object->flush();}
  bool write(const void* const buffer, size_t size) {return object->write(buffer, size);}
  bool setPosition (int64 newPosition) {return object->setPosition(newPosition);}
  int64 getPosition() {return object->getPosition();}

private:
  juce::OutputStream *object;
};

#endif
