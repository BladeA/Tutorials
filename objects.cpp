//
// Created by paul on 8/1/16.
//

#include "objects.h"

/*
 * Universe class functions
 *
 * The Universe is a class which keeps track of all objects in the game.
 * Also it will contain the physics engine etc.
 *
 * It has functions for adding objects, removing objects, getting objects, etc.
 *
 */

// Constructor, currently not used
Universe::Universe() : height(_Height), width(_Width) {
    this->_Width = 640;
    this->_Height = 480;
}

Universe::Universe(double Width, double Height) : height(_Height), width(_Width){
    this->_Width = Width;
    this->_Height = Height;
}

/*
 * add_object(Object &obj)
 *
 * Add a copy of obj to the objects list of the current universe. Returns the id of the object
 * in the universe.
 */
id_type Universe::add_object (Object &obj) {
    // Add a copy of the object to the vector of objects in the universe.
    objects.push_back(obj);

    // Get a reference to this copy
    Object* new_obj = &objects[objects.size()-1];

    // Give the object a unique ID
    id_type new_id = this->give_new_object_id();
    new_obj->set_id(new_id);

    // Update the id to object mapping
    this->update_id_to_object_map();

    // Return the new object's id
    return new_obj->get_id();
}

// Remove object functions

/*
 * remove_object_by_index(int obj_index)
 *
 * The fastest way to remove an object from the universe. Removes the object
 * which is at index obj_index in the vector of objects. Shifts all other
 * objects to make the list continuous again. (uses std::vector.erase() to do so)
 *
 * Calls update_id_to_object_map() at the end
 */
void Universe::remove_object_by_index(int obj_index) {
    // Remove the object from the vector of objects
    objects.erase(objects.begin() + obj_index);

    // Update the id mapping
    this->update_id_to_object_map();
}

/*
 * remove_object_by_id(id_type id)
 *
 * This function removes the object given the id. First has to find the object,
 * then notes the index and calles remove_object_by_index() to do the dirty work.
 *
 * Returns true on success, false on failure.
 */
bool Universe::remove_object_by_id(id_type id) {
    // Loop through all objects, until we have found the correct object
    int delete_index = 0;
    bool found_it = false;

    for (int ii = 0; ii < objects.size(); ++ii) {

        if ( objects[ii].get_id() == id ) {
            // We found the correct one! Store the index
            found_it = true;
            delete_index = ii;
            break;
        }
    }

    // Remove it if it was found
    if (found_it) {
        remove_object_by_index(delete_index);
        return true;
    }
    else{
        // Whoops, couldn't find it!
        std::cerr << "[WARN] Cannot delete object with id " << id << " because couldn't find it!" << std::endl;
        return false;
    }
}

/*
 * give_new_object_id()
 *
 * Function which returns a unique new id number of type id_type.
 */
id_type Universe::give_new_object_id() {
    return ++new_object_id;
}


// Get object functions

/*
 * get_object_by_index()
 *
 * Returns the object pointer in this universe by index number. Usage of this function
 * is not recommended, use get_object_by_id() instead.
 */
Object* Universe::get_object_by_index(int index) {
    return &objects[index];
}

/*
 * get_object_by_id()
 *
 * Returns the object pointer given the object id number. Uses the
 * id_to_object_map, generated by update_id_to_object_map().
 */
Object* Universe::get_object_by_id(id_type id) {
    return id_to_object_map[id];
}

/*
 * get_all_objects()
 *
 * Returns the vector of all objects in the universe.
 */
std::vector<Object> Universe::get_all_objects() {
    return objects;
}

/*
 * update_id_to_object_map()
 *
 * Generates a mapping from id to object pointer. Not a very quick function,
 * only call when updating the universe list of objects.
 */
void Universe::update_id_to_object_map() {
    // Clear the current mapping
    id_to_object_map = {};

    // Loop through all objects stored in the universe
    for (int ii = 0; ii < objects.size(); ii++) {
        // Create a mapping from the object id to the object pointer
        id_to_object_map[ objects[ii].get_id() ] = &objects[ii];
    }
}











// Object


/*
 * set_position()
 *
 * Sets the position of the object to the new two double input parameters.
 */
void Object::set_position(double new_x, double new_y) {
    // IMPLEMENT INPUT SANITATION CHECKS HERE?
    position[0] = new_x;
    position[1] = new_y;
}

void Object::set_position(vec2d new_pos) {
    position = new_pos;
}

/*
 * get_position()
 *
 * Gets the position of the object, returns array of two doubles.
 */
vec2d Object::get_position () {
    return position;
}

/*
 * set_velocity()
 *
 * Sets the velocity of the object
 */
void Object::set_velocity(double new_vx, double new_vy) {
    // IMPLEMENT INPUT SANITATION CHECKS HERE?
    velocity[0] = new_vx;
    velocity[1] = new_vy;
}
void Object::set_velocity(vec2d new_v) {
    velocity = new_v;
}

/*
 * get_velocity()
 *
 * Returns the object's velocity as an array of two doubles
 */
vec2d Object::get_velocity () {
    return velocity;
}

/*
 * set_id()
 *
 * This function can be called once after the object is created. Sets the id
 * of the object to the input parameter of this function.
 */
void Object::set_id(id_type &new_id) {
    if ( ! this->id_set ) {
        this->id = new_id;
        this->id_set = true;
    }
    else{
        // Attempted to re-set the id!
        std::cerr << "[WARN] Object::set_id() called to set the id to " << new_id << ", but it is already set to " << this->id << std::endl;
    }

}

/*
 * get_id()
 *
 * Returns the id of this object.
 */
id_type Object::get_id() {
    return id;
}

double Object::get_mass() {
    return mass;
}

void Object::set_mass(double m) {
    if ( m > 0 ) {
        mass = m;
    }
    else{
        std::cout << "Attempting to set the mass of an object <= 0!";
    }
}

double Object::get_radius() {
    return radius;
}

void Object::set_radius(double r){
    if( r > 0 ){
        radius = r;
    } else{
        std::cerr << "[WARN] Tried to set radius of object #" << id << " to invalid value" << r << std::endl;
    }
}


bool Universe::check_collision(Object* A, Object* B) {
    vec2d A_v = A->get_velocity();
    vec2d A_x = A->get_position();

    vec2d B_v = B->get_velocity();
    vec2d B_x = B->get_position();

    double A_m = A->get_mass();
    double B_m = B->get_mass();

    double A_r = A->get_radius();
    double B_r = B->get_radius();

    // Check if objects are overlapping and moving aay from each other
    if(len(sub(A_x,B_x)) < (A_r+B_r) && dot(sub(A_v,B_v),sub(B_x,A_x)) > 0){
        return true;
    } else{
        return false;
    }
}

void Universe::resolve_collision(Object* A, Object* B) {
    // Using the math from Wikipedia: https://en.wikipedia.org/wiki/Elastic_collision
    vec2d A_v = A->get_velocity();
    vec2d A_x = A->get_position();

    vec2d B_v = B->get_velocity();
    vec2d B_x = B->get_position();

    double A_m = A->get_mass();
    double B_m = B->get_mass();

    // Horrible long expression which calculates the new position! It is the equation on Wikipedia for the
    // vector notation of resolving a collision.

    vec2d A_v_new = sub(A_v, cmult(sub(A_x, B_x), (2*B_m/(A_m + B_m))*dot(sub(A_v, B_v), sub(A_x, B_x)) / len_squared(sub(A_x, B_x))));
    vec2d B_v_new = sub(B_v, cmult(sub(B_x, A_x), (2*A_m/(A_m + B_m))*dot(sub(B_v, A_v), sub(B_x, A_x)) / len_squared(sub(B_x, A_x))));

    // Push these new vectors to the objects
    A->set_velocity(A_v_new);
    B->set_velocity(B_v_new);

}

/*
 * physics_runtime_iteration()
 *
 * Call this function to do an update of position and velocity of all objects in this universe.
 * The idea is that the main runtime can call this function to perform the subset of functions
 * which is needed to do the physics engine.
 */
void Universe::physics_runtime_iteration () {
    // Temporary result storage
    std::map<Object*,std::array<vec2d, 2>> new_pos_vel_universe;

    // Iterate over all objects
    for (int ii = 0; ii < objects.size(); ++ii) {
        std::array<vec2d, 2> new_pos_vel = objects[ii].calc_new_pos_vel(objects, this->timestep);
        new_pos_vel_universe[&objects[ii]] = new_pos_vel;
    }

    // Now update the positions and velocities of all objects
    for (int ii = 0; ii < objects.size(); ++ii) {
        objects[ii].set_position(new_pos_vel_universe[&objects[ii]][0]);
        objects[ii].set_velocity(new_pos_vel_universe[&objects[ii]][1]);
    }

    // Check for collisions
    for (int ii = 0; ii < objects.size(); ++ii) {
        for (int jj = 0; jj < objects.size(); ++jj) {
            // Do not check yourself
            if (ii != jj) {
                // Check for a collision
                if ( check_collision(&objects[ii], &objects[jj]) ) {
                    // If that is the case, go fix it!
                    this->resolve_collision(&objects[ii], &objects[jj]);
                }
            }
        }
    }

}

/*
 * calc_new_pos_vel()
 *
 * Calculate the new position of the object by doing a Euler algorithm time step.
 */
std::array<vec2d, 2> Object::calc_new_pos_vel(std::vector<Object> &objects, double &timestep) {
    // Without acceleration
    std::array<vec2d, 2> new_pos_vel;

    new_pos_vel[0] = add(position, cmult(velocity, timestep));
    new_pos_vel[1] = velocity;

    return new_pos_vel;
};


double Universe::distance_between(Object* A, Object* B) {
    vec2d pos_A = A-> get_position();
    vec2d pos_B = B-> get_position();
    double dist = std::sqrt(((pos_B[0]-pos_A[0])*(pos_B[0]-pos_A[0]))+((pos_B[1]-pos_A[1])*(pos_B[1]*pos_A[1])));
    return dist;
}