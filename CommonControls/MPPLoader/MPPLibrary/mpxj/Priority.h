#pragma once

/**
 * This class is used to represent a priority. It provides a mapping
 * between the textual description of a priority found in an MPX
 * file, and an enumerated representation that can be more easily manipulated
 * programatically.
 */
class COMMONCONTROLS_API Priority : public Object2
{
private:
   /**
    * This constructor takes the numeric enumerated representation of a
    * priority and populates the class instance appropriately.
    * Note that unrecognised values are treated as medium priorities.
    *
    * @param priority int representation of the priority
    */
	Priority(int priority);

public:
   /**
    * This method takes an integer enumeration of a priority
    * and returns an appropriate instance of this class. Note that unrecognised
    * values are treated as medium priority.
    *
    * @param priority int version of the priority
    * @return Priority class instance
    */
	static std::shared_ptr<Priority> getInstance(int priority);

   /**
    * Accessor method used to retrieve the numeric representation of the
    * priority.
    *
    * @return int representation of the priority
    */
	int getValue();

   /**
    * Constant for lowest priority.
    */
	static const int LOWEST = 100;

   /**
    * Constant for low priority.
    */
   static const int VERY_LOW = 200;

   /**
    * Constant for lower priority.
    */
   static const int LOWER = 300;

   /**
    * Constant for low priority.
    */
   static const int LOW = 400;

   /**
    * Constant for medium priority.
    */
   static const int MEDIUM = 500;

   /**
    * Constant for high priority.
    */
   static const int HIGH = 600;

   /**
    * Constant for higher priority.
    */
   static const int HIGHER = 700;

   /**
    * Constant for very high priority.
    */
   static const int VERY_HIGH = 800;

   /**
    * Constant for highest priority.
    */
   static const int HIGHEST = 900;

   /**
    * Constant for do not level.
    */
   static const int DO_NOT_LEVEL = 1000;

private:
   /**
    * Array of type values matching the above constants.
    */
	static std::shared_ptr<Priority> VALUE[10];

   /**
    * Internal representation of the priority.
    */
   int m_value;
};

typedef std::shared_ptr<Priority> PriorityPtr;