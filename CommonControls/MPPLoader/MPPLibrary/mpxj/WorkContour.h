#pragma once
/**
 * Instances of this class represent enumerated work contour values.
 */
enum WorkContour// implements MpxjEnum
{
	FLAT=0,
	BACK_LOADED=1,
	FRONT_LOADED=2,
	DOUBLE_PEAK=3,
	EARLY_PEAK=4,
	LATE_PEAK=5,
	BELL=6,
	TURTLE=7,
	CONTOURED=8
};
//   /**
//    * Private constructor.
//    * 
//    * @param type int version of the enum
//    */
//   private WorkContour(int type)
//   {
//      m_value = type;
//   }
//
//   /**
//    * Retrieve an instance of the enum based on its int value.
//    *
//    * @param type int type
//    * @return enum instance
//    */
//   public static WorkContour getInstance(int type)
//   {
//      if (type < 0 || type >= TYPE_VALUES.length)
//      {
//         type = FLAT.getValue();
//      }
//      return (TYPE_VALUES[type]);
//   }
//
//   /**
//    * Retrieve an instance of the enum based on its int value.
//    *
//    * @param type int type
//    * @return enum instance
//    */
//   public static WorkContour getInstance(NumberPtr type)
//   {
//      int value;
//      if (type == NULL)
//      {
//         value = -1;
//      }
//      else
//      {
//         value = NumberHelper::getInt(type);
//      }
//      return (getInstance(value));
//   }
//
//   /**
//    * Accessor method used to retrieve the numeric representation of the enum. 
//    *
//    * @return int representation of the enum
//    */
//   @Override public int getValue()
//   {
//      return (m_value);
//   }
//
//   /**
//    * Array mapping int types to enums.
//    */
//   private static final WorkContour[] TYPE_VALUES = EnumHelper.createTypeArray(WorkContour.class);
//
//   /**
//    * Internal representation of the enum int type.
//    */
//   private int m_value;
//}
