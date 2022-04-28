template <class Item, class Iterator>
class BackTrack {

public:

   enum SolutionType {FirstSolution, NextSolution};

   BackTrack (Item First,    // first Item value
              Item Last);    // last Item value

   virtual ~BackTrack();


   // Finds the next solution to the problem (if one exists).
   // Repeated calls will find all solutions to a problem if
   // multiple solutions exist.
   //
   void operator()
      (Iterator     Start,
       Iterator     End,
       SolutionType Type,
       bool&        Valid);


protected:


   // Increments Start.
   //
   // This class guarantees that the input parameter *Start will
   // always be in the range:
   //      First <= X < Last,
   //
   // where First and Last are the values passed to the BackTrack
   // constructor, so no range checking is necessary.
   //
   virtual void Succ (Iterator Start) {(*Start)++;}


   // Solution evaluator. Evaluates [Start..End) for correctness
   // and returns true if and only if it is valid.
   //
   virtual bool IsValid (Iterator Start, Iterator End) = 0;


private:

   Item FirstValue;
   Item LastValue;

   // Assigns the first value of Item that results in a valid
   // solution to the last element of [Start..End). Sets output
   // parameter Valid to true if a Valid solution was found.
   //
   void NextValidValue  (Iterator Start, Iterator End, bool& Valid);


   // Backtracks through the solution [Start..End) until it finds a
   // node != last value of Item and sets it to the next possible
   // value (as defined by Succ()), setting End to the end of the
   // new solution. Valid is set true if and only if an unvisited
   // node is found (thus false implies that all possible solutions
   // have been generated).
   //
   void GotoUnvisitedNode
        (Iterator Start, Iterator& End, bool& Found);

};


template <class Item, class Iterator>
BackTrack<Item,Iterator>::BackTrack
(Item First,
 Item Last)
{
   FirstValue  = First;
   LastValue = Last;
}


template <class Item, class Iterator>
BackTrack<Item,Iterator>::~BackTrack ()
{
}


template <class Item, class Iterator>
void BackTrack<Item,Iterator>::GotoUnvisitedNode
(Iterator  Start,
 Iterator& End,
 bool&     Found)
{
   Iterator Last = End-1;

   // Back up as long as we have already exhausted all possible
   // settings for the current node.
   //
   while (Last != Start && *Last == LastValue)
     Last--;

   if (*Last != LastValue) {
      Succ (Last);
      Found = true;
   } else
     Found = false;

   End = Last+1;
}


template <class Item, class Iterator>
void BackTrack<Item,Iterator>::NextValidValue
(Iterator Start,
 Iterator End,
 bool&    Valid)
{
   Valid = false;
   Iterator Last = End-1;

   bool CheckNode = true;
   while (CheckNode)
     if (IsValid (Start, End)) {
        Valid = true;
        CheckNode = false;
     } else if (*Last != LastValue)
       Succ (Last);
     else
       CheckNode = false;
}


template <class Item, class Iterator>
void BackTrack<Item,Iterator>::operator()
(Iterator     Start,
 Iterator     End,
 SolutionType Type,
 bool&        Valid)
{
   if (Start >= End) {
      Valid = false;
      return;
   }

   bool SolutionValid = true;
   Iterator SolutionEnd = End;

   if (Type == FirstSolution) {
      // Initialize to the first possible permutation.
      End    = Start;
      *Start = FirstValue;
   } else
     // Move past the valid solution to the next unvisited node.
     GotoUnvisitedNode (Start, End, SolutionValid);

   while (SolutionValid) {
      NextValidValue (Start, End, SolutionValid);

      if (SolutionValid)
        if (End == SolutionEnd)  // Found full solution?
          break;               // yes - quit searching.
        else
          // Create leftmost child of this node
          *(End++) = FirstValue;
      else
        GotoUnvisitedNode (Start, End, SolutionValid);
   }
   Valid = SolutionValid;
}


