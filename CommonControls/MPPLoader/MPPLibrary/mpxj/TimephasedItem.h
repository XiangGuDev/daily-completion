#pragma once

/**
 * This class represents an amount, spread over a period of time.
 * 
 * @param <T> payload type
 */
template <class T>
class TimephasedItem
{
public:
   /**
    * Retrieve the start date.
    * 
    * @return start date
    */
   DatePtr getStart()
   {
      return m_start;
   }

   /**
    * Set the start date.
    * 
    * @param start start date
    */
   void setStart(DatePtr start)
   {
      m_start = start;
   }

   /**
    * Retrieve the amount per day.
    * 
    * @return amount per day
    */
   T getAmountPerDay()
   {
      return m_amountPerDay;
   }

   /**
    * Set the amount per day.
    * 
    * @param amountPerDay amount per day
    */
   void setAmountPerDay(T amountPerDay)
   {
      m_amountPerDay = amountPerDay;
   }

   /**
    * Retrieve the modified flag.
    * 
    * @return modified flag
    */
   bool getModified()
   {
      return m_modified;
   }

   /**
    * Set the modified flag. 
    * 
    * @param modified modified flag
    */
   void setModified(bool modified)
   {
      m_modified = modified;
   }

   /**
    * Retrieve the total amount.
    * 
    * @return total amount
    */
   T getTotalAmount()
   {
      return m_totalAmount;
   }

   /**
    * Set the total amount.
    * 
    * @param totalAmount total amount
    */
   void setTotalAmount(T totalAmount)
   {
      m_totalAmount = totalAmount;
   }

   /**
    * Retrieve the finish date.
    * 
    * @return finish date
    */
   DatePtr getFinish()
   {
      return m_finish;
   }

   /**
    * Set the finish date.
    * 
    * @param finish finish date
    */
   void setFinish(DatePtr finish)
   {
      m_finish = finish;
   }

   ///**
   // * {@inheritDoc}
   // */
   //@Override public CString toString()
   //{
   //   return "[TimephasedItem start=" + m_start + " totalAmount=" + m_totalAmount + " finish=" + m_finish + " amountPerDay=" + m_amountPerDay + " modified=" + m_modified + "]";
   //}

   /**
    * {@inheritDoc}
    */
   virtual bool equals(TimephasedItem<T>* o)
   {
      bool result = false;

      //if (o instanceof TimephasedItem<?>)
      //{
	  TimephasedItem<T>* t = o;// (TimephasedItem<T>) o;
	  result = m_start->equals(t->m_start.get()) && m_finish->equals(t->m_finish.get()) &&
		  m_totalAmount->equals(t->m_totalAmount.get()) && m_amountPerDay->equals(t->m_amountPerDay.get());
      //}

      return result;
   }

   ///**
   // * {@inheritDoc}
   // */
   //@Override public int hashCode()
   //{
   //   return m_start.hashCode() + m_finish.hashCode() + m_totalAmount.hashCode() + m_amountPerDay.hashCode();
   //}

private:
	DatePtr m_start;
	T m_totalAmount;
	DatePtr m_finish;
	T m_amountPerDay;
	bool m_modified;
};