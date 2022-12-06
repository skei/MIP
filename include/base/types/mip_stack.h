#ifndef mip_stack_included
#define mip_stack_included
//----------------------------------------------------------------------

/*
  - primitive
  - no error checking (yet)
*/

//----------------------------------------------------------------------

template<class _T,int SIZE>
class MIP_Stack
{

//------------------------------
private:
//------------------------------

  _T  MBuffer[SIZE] = {0};
  int MIndex        = 0;

//------------------------------
public:
//------------------------------

  MIP_Stack() {
    reset();
  }

  //----------

  ~MIP_Stack() {
  }

//------------------------------
public:
//------------------------------

  void reset(void) {
    MIndex = 0;
  }

  //----------

  int getSize() {
    return SIZE;
  }

  //----------

  int getNumItems() {
    return MIndex;
  }

  //----------

  bool isEmpty() {
    return (MIndex > 0);
  }

  //----------

  _T* getTop(void) {
    return &MBuffer[MIndex-1];
  }

  //----------

  // c  ->  a b -  ->  c a b -

  void push(_T AItem) {
    MBuffer[MIndex] = AItem;
    MIndex++;
    //if (MIndex > SIZE) printf("stack overrun!\n");
  }

  //----------

  // a b -  ->  b - -  -> [a]

  _T pop(void) {
    if (MIndex == 0) printf("empty stack!\n");
    MIndex--;
    return MBuffer[MIndex];
  }

  //----------

  // a b -  ->  a a b -

  void dup(void) {
    MBuffer[MIndex] = MBuffer[MIndex-1];
    MIndex++;
  }

  //----------

  // a b -  ->  b a b -

  void dup2(void) {
    MBuffer[MIndex] = MBuffer[MIndex-2];
    MIndex++;
  }

  //----------

  // a b -  ->  b - -  ->

  void drop(void) {
    MIndex--;
  }

  //----------

};

//----------------------------------------------------------------------
#endif
