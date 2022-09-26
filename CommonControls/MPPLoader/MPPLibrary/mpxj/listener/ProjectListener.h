#pragma once

class Resource2;
class Task;
class Relation;
class ProjectCalendar;
class Resource2Assignment;

/**
 * Classes implementing this interface can be used to receive notification
 * of events occurring within the project file->
 */
class ProjectListener
{
public:
   /**
    * This method is called when a task is read from a file->
    *
    * @param task task instance
    */
   virtual void taskRead(Task* task) = 0;

   /**
    * This method is called when a task is written to a file->
    *
    * @param task task instance
    */
   virtual void taskWritten(Task* task) = 0;

   /**
    * This method is called when a resource is read from a file->
    *
    * @param resource resource instance
    */
   virtual void resourceRead(Resource2* resource) = 0;

   /**
    * This method is called when a resource is written to a file->
    *
    * @param resource resource instance
    */
   virtual void resourceWritten(Resource2* resource) = 0;

   /**
    * This method is called when a calendar is read from a file->
    *
    * @param calendar calendar instance
    */
   virtual void calendarRead(ProjectCalendarPtr calendar) = 0;

   /**
    * This method is called when a calendar is written to a file->
    * 
    * @param calendar calendar instance
    */
   virtual void calendarWritten(ProjectCalendarPtr calendar) = 0;

   /**
    * This method is called when an assignment is read from a file->
    *
    * @param assignment resource assignment
    */
   virtual void assignmentRead(Resource2Assignment* assignment) = 0;

   /**
    * This method is called when an assignment is written to a file->
    * 
    * @param assignment assignment instance
    */
   virtual void assignmentWritten(Resource2Assignment* assignment) = 0;

   /**
    * This method is called when a relation is read from a file->
    *
    * @param relation relation instance
    */
   virtual void relationRead(RelationPtr relation) = 0;

   /**
    * This method is called when a relation is written to a file->
    * 
    * @param relation relation instance
    */
   virtual void relationWritten(RelationPtr relation) = 0;
};