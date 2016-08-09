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

// Constructors

/*
 * Note on how the constructors are used: We implemented a code sequence to get "read-only" parameters.
 * The idea is that you can access properties like they are public (read them), but cannot change their
 * value, unless you are a member function of the class. See the example at http://www.cplusplus.com/forum/articles/36872/
 * or copied here below:
 *
 * class A
 * {
 *     int _x; // Private variable, it can modify in this class directly
 * public:
 *     A() : x(_x) // Bind reference variable x to _x
 *     {
 *     }
 *
 *     void setX(int x)
 *     {
 *         this->_x = x;
 *     }
 *
 *     const int &x; // Constant variable
 * };
 *
 * // Usage:
 *
 * int main()
 * {
 *     A a;
 *
 *     a.setX(50);
 *
 *     cout << a.x << endl; // Correct
 *     a.x = 12; // Error !
 *
 *     return 0;
 * }
 */
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
void Object::set_colour(std::array<double, 4> Colour){
    for(int ii = 0; ii < colour.size(); ii++){
        if(Colour[ii]<0.0){
            colour[ii] = 0.0;
            std::cerr << "[WARN] Tried to set a colour value of object #" << id << " to invalid value" << Colour[ii] << " colour value was forced to 0" << std::endl;
        }else if(Colour[ii]<=1.0) {
            colour[ii] = Colour[ii];
        }else{
            colour[ii] = 1.0;
            std::cerr << "[WARN] Tried to set a colour value of object #" << id << " to invalid value" << Colour[ii] << " colour value was forced to 1" << std::endl;
        }
    }
};
std::array<double, 4> Object::get_colour(){
    return colour;
};

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
    if(len(sub(A_x,B_x)) < (A_r+B_r) && A_x != B_x && dot(sub(A_v,B_v),sub(B_x,A_x)) > 0){
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

    double coeff; // Coefficient of restitution, the smallest of the two
    if ( A->bouncyness < B->bouncyness )
        coeff = A->bouncyness;
    else
        coeff = B->bouncyness;

    /*
     * For the collision you can have 100% elastic or 100% inelastic. The final velocity vector is a
     * linear combination of the two, where the bouncyness coefficient determines how much elastic
     * the collision is.
     *
     * Not completely sure energy is conserved with this method :S
     */

    // Initialise the vectors
    vec2d A_v_e_new; // Object A, velocity, elastic, new
    vec2d A_v_i_new; // Object A, velocity, inelastic, new
    vec2d B_v_e_new; // etc
    vec2d B_v_i_new;

    // Horrible long expression which calculates the new velocity! It is the equation on Wikipedia for the
    // vector notation of resolving a collision: https://en.wikipedia.org/wiki/Elastic_collision
    A_v_e_new = sub(A_v, cmult(sub(A_x, B_x),
                                     (coeff) * (2 * B_m / (A_m + B_m)) * dot(sub(A_v, B_v), sub(A_x, B_x)) /
                                     len_squared(sub(A_x, B_x))));
    B_v_e_new = sub(B_v, cmult(sub(B_x, A_x),
                                     (coeff) * (2 * A_m / (A_m + B_m)) * dot(sub(B_v, A_v), sub(B_x, A_x)) /
                                     len_squared(sub(B_x, A_x))));

    // Fully inelastic expression, very simple derivation:
    /*
     * p momentum, v velocity, m mass, ' final state
     * p1' + p2' = p1 + p2 // Conservation of momentum
     * v1' = v2'           // Same final velocity
     * --> v1' = p1 + p2/(m1 + m2)
     */
    A_v_i_new = cmult(add(cmult(A_x, A_m), cmult(B_x, B_m)), 1/(A_m + B_m));
    B_v_i_new = A_v_i_new; // Same velocity, fully joined together in motion

    // Create a linear combination of the two types of collision
    vec2d A_v_new = add(cmult(A_v_e_new, coeff), cmult(A_v_i_new, (1-coeff)));
    vec2d B_v_new = add(cmult(B_v_e_new, coeff), cmult(B_v_i_new, (1-coeff)));

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
        std::array<vec2d, 2> new_pos_vel = objects[ii].calc_new_pos_vel(objects, this->timestep, this->physics);
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

        // Check if we are near a wall
        vec2d pos = objects[ii].get_position();
        double r = objects[ii].get_radius();

        // If it exceeds the world
        if (    pos[0] - r < -this->_Width/2  || pos[0] + r > this->_Width/2 || \
                pos[1] - r < -this->_Height/2 || pos[1] + r > this->_Height/2 ) {
            // Make a mirror object at the
        }

        //// Durp wall collision is rather ugly now, put in subroutine

        // Colliding in the west wall
        if ( pos[0] - r < -this->_Width/2 ) {
            // Create a mirror object
            Object mirror;
            mirror.set_mass(objects[ii].get_mass());
            mirror.set_radius(objects[ii].get_radius());
            mirror.bouncyness = objects[ii].bouncyness;

            // Preserve the vertical velocity, inverse the horizontal velocity
            vec2d vel = objects[ii].get_velocity();
            mirror.set_velocity(-1*vel[0], vel[1]);

            // Set the same vertical position, mirrored horizontal in the wall
            mirror.set_position(-this->_Width/2 + (this->_Width/2-pos[0]), pos[1]);

            // Perform a collision
            this->resolve_collision(&mirror, &objects[ii]);

            // Mirror object will clean up due to scope
        }

        // Colliding into the east wall
        if ( pos[0] + r > this->_Width/2 ) {
            // Create a mirror object
            Object mirror;
            mirror.set_mass(objects[ii].get_mass());
            mirror.set_radius(objects[ii].get_radius());
            mirror.bouncyness = objects[ii].bouncyness;

            // Preserve the vertical velocity, inverse the horizontal velocity
            vec2d vel = objects[ii].get_velocity();
            mirror.set_velocity(-1*vel[0], vel[1]);

            // Set the same vertical position, mirrored horizontal in the wall
            mirror.set_position(this->_Width/2 + (this->_Width/2 - pos[0]), pos[1]);

            // Perform a collision
            this->resolve_collision(&mirror, &objects[ii]);

            // Mirror object will clean up due to scope
        }

        // Collide into the north wall
        if ( pos[1] + r > this->_Height/2 ) {
            // Create a mirror object
            Object mirror;
            mirror.set_mass(objects[ii].get_mass());
            mirror.set_radius(objects[ii].get_radius());
            mirror.bouncyness = objects[ii].bouncyness;

            // Preserve the horizontal velocity, inverse the vertical velocity
            vec2d vel = objects[ii].get_velocity();
            mirror.set_velocity(vel[0], -1*vel[1]);

            // Set the same horizontal position, mirrored vertical in the wall
            mirror.set_position(pos[0], this->_Height/2 + (this->_Height/2 - pos[1]));

            // Perform a collision
            this->resolve_collision(&mirror, &objects[ii]);

            // Mirror object will clean up due to scope
        }

        // Collide into the south wall
        if ( pos[1] - r < -this->_Height/2 ) {
            // Create a mirror object
            Object mirror;
            mirror.set_mass(objects[ii].get_mass());
            mirror.set_radius(objects[ii].get_radius());
            mirror.bouncyness = objects[ii].bouncyness;

            // Preserve the horizontal velocity, inverse the vertical velocity
            vec2d vel = objects[ii].get_velocity();
            mirror.set_velocity(vel[0], -1*vel[1]);

            // Set the same horizontal position, mirrored vertical in the wall
            mirror.set_position(pos[0], -this->_Height/2 + (-this->_Height/2 - pos[1]));

            // Perform a collision
            this->resolve_collision(&mirror, &objects[ii]);

            // Mirror object will clean up due to scope
        }
    }

}

/*
 * calc_new_pos_vel()
 *
 * Calculate the new position of the object by doing a Euler algorithm time step.
 */
std::array<vec2d, 2> Object::calc_new_pos_vel(std::vector<Object> &objects, double &time_step, Physics &physics) {
    // Initialize the result array
    std::array<vec2d, 2> new_pos_vel;

    // Calculate the acceleration
    vec2d acceleration = {0,0};
    Object* X = this;
    // Loop through all objects
    for (int ii = 0; ii < objects.size(); ++ii ) {
        // Make sure you are not calculating yourself
        if ( objects[ii].get_id() == this-> get_id()) {
            // This is myself, skip this loop iteration
            continue;

        }

        vec2d this_acc = physics.acceleration(X, &objects[ii]);
        // Here add up the contribution to the acceleration
        acceleration = add(acceleration, this_acc);
    }

    new_pos_vel[0] = add(position, cmult(velocity, time_step));
    new_pos_vel[1] = add(velocity, cmult(acceleration, time_step));

    return new_pos_vel;
};


double Physics::distance_between(Object* X, Object* Y) {
    vec2d pos_X = X-> get_position();
    vec2d pos_Y = Y-> get_position();
    double dist = std::sqrt(((pos_Y[0]-pos_X[0])*(pos_Y[0]-pos_X[0]))+((pos_Y[1]-pos_X[1])*(pos_Y[1]*pos_X[1])));
    return dist;
}

vec2d Physics::acceleration (Object* X, Object* Y){
    double dist = this -> distance_between (X , Y);
    vec2d pos_X = X -> get_position();
    vec2d pos_Y = Y -> get_position();
    vec2d r = sub(pos_Y , pos_X);
    double mass = Y -> get_mass();
    vec2d acc = cmult(r,(mass/(dist*dist*dist)));
    return acc;
}