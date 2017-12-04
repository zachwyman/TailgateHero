#include <vector>
#include <list>
#include "chunk.h"
#include "multisprite.h"

class ExplodingMultiSprite : public MultiSprite {
public:
  ExplodingMultiSprite(const MultiSprite& s);
  ~ExplodingMultiSprite();
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  void makeChunks(unsigned int);
  unsigned int chunkCount() const { return chunks.size(); }
  unsigned int freeCount()  const { return freeList.size(); }
private:
  std::list<Chunk*> chunks; // An ExplodingSprite is a list of sprite chunks
  std::list<Chunk*> freeList; // When a chunk gets out of range it goes here

  ExplodingMultiSprite(const ExplodingMultiSprite&); // Explicit disallow (Item 6)
  ExplodingMultiSprite& operator=(const ExplodingMultiSprite&); // (Item 6)
};
