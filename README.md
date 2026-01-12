# ilib

My own custom library I use for all of my current TF2 plugins. 

My plugins will not compile without these dependencies, just include ilib and it will import the rest.

The library includes objects (structs/methodmaps) which can be used to wrap clients/entities more conveniently. 

The two main structs provided:

### FObject
 - Can be used to wrap any entity
 - Validity checks and other common functions are provided with this object
 - Index/Reference of the entity can be pulled with the respective getters
     - `FObject::Get()`
     - `FObject::GetReference()`

### FClient
 - Similar to `FObject` but only used for wrapping Client entities
 - Client specific functions are provided with this object

## Objects addon
TF2Attributes is required if using the optional ilib_objects include.
