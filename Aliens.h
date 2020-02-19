//Thanks to @Pharap for this code which I found on the Arduboy forum
#pragma once

class Alien
{
    public:
        int8_t area;
        int8_t zone;

    public:
        Alien(void) = default;
        
        Alien(int8_t area, int8_t zone)
        {
            this->area = area;
            this->zone = zone;
        }
};


// Now create a List collection to hold all the aliens
//template<int N>
template<size_t capacity>
struct AlienList
{
    private:
        Alien array[capacity];
        uint8_t next;

    public:	
        // Returns the number of Enemys currently in the list
        uint8_t getCount() const { return next; }
        
        // Returns the maximum number of Enemys the list can hold
        uint8_t getCapacity() const { return capacity; }
        
        // Returns true if the list is full
        bool isFull() const { return getCount() == getCapacity(); }
        
        // Returns true if the list is empty
        bool isEmpty() const { return getCount() == 0; }
        
        // Clears the list (by cheating)
        void clear() { next = 0; }
        
        // Returns true if the Enemy was added
        // Returns false if the list is already full
        bool add(const Alien & alien)
        {
            if(this->isFull())
                return false;
            
            array[next] = alien; // put the Enemy in the array
            ++next; // increment the next index
            return true;
        }
        
        // Returns true if the Enemy was removed
        // Returns false if the index was invalid
        bool removeAt(uint8_t index)
        {
            if(index >= next)
                return false;
            
            --next; // decrement next index
            for(int i = index; i < next; ++i) // shuffle everything down
                array[i] = array[i + 1];
            return true;
        }
        
        // These are for indexing the list
        // Be careful, these don't check if the index is valid
        Alien & operator[](uint8_t index) { return array[index]; }	
        const Alien & operator[](uint8_t index) const { return array[index]; }
};