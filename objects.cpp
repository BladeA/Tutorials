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
Universe::Universe() {

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
    mass = m;
}

void Object::resolve_collision(Object* A, Object* B) {
    // Using the math from Wikipedia: https://en.wikipedia.org/wiki/Elastic_collision
    vec2d A_v = A->get_velocity();
    vec2d A_x = A->get_position();

    vec2d B_v = B->get_velocity();
    vec2d B_x = B->get_position();

    vec2d A_m = A->get_mass();
    vec2d B_m = B->get_mass();

    // Horrible long expression which calculates the new position! It is the equation on Wikipedia for the
    // vector notation of resolving a collision.
    vec2d A_v_new = sub(A_v, cmult(sub(A_x, B_x), (2*B_m/(A_m + B_m))*dot(sub(A_v, B_v), sub(A_x, B_x)) / len_squared(sub(A_x, B_x))));
    vec2d B_v_new = sub(B_v, cmult(sub(B_x, A_x), (2*A_m/(A_m + B_m))*dot(sub(B_v, A_v), sub(B_x, A_x)) / len_squared(sub(B_x, A_x))));

    // Push these new vectors to the objects
    A->set_velocity(A_v_new);
    B->set_velocity(B_v_new);

}