# Raven Primitives
---
## Primitives Types
Each primitive type in Raven alongside it's equivalent in C.
### Numeric 
#### Integers
##### Signed
* `i8`  ( `int8_t` )
* `i16` ( `int16_t` )
* `i32` ( `int32_t` )
* `i64` ( `int64_t` )
##### Unsigned
* `u8`  ( `uint8_t` )
* `u16` ( `uint16_t` )
* `u32` ( `uint32_t` )
* `u64` ( `uint64_t` )
#### Floats
* `f32` ( `float` )
* `f64` ( `double` )
### Boolean
* `bool` ( `bool` *in C++*)
### Pointers
* `&` character marks a type as a reference
  * `var ptr: &u8`
### Text
* `char` ( `char` )
* `String` ( `char*` )
---
## Interations between primitivies
### Numeric <-> Numeric
Numeric primitives can interact with each other but a warning will be presented to the user to inform them that there is a mismatch in the types but if the result of the interaction is not of the same type as a variable a semantic error will be raised.
During an interaction between two numeric data types the smaller of the two will be cast to the size of the larger.
#### Float <-> Integer
Floats and integers must be cast to either or the other before an interaction will be viable.
### Numeric <-> Pointers
Unsure if pointer arithmetic will be used in Raven.
### Numeric <-> Text
Integers, both signed and unsigned can be used in the indexing of strins, or the manipulation of characters.