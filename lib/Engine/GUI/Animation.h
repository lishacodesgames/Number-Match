namespace GUI
{
   /**
    * LINEAR INTERPOLATION
    * 
    * Moves 10% of remaining distance till it's too close
    * then it snaps to destination
    * 
    * @param elmPos current element position
    * @param destination final position of element
    * @param speedFactor how much this frame should affect element's position
    * @param minDistance minimum distance (in px) till elmPos snaps to destination
    * 
    * @return new element position
    */
   float LERP(float elmPos, float destination, float speedFactor, float minDistance);
}