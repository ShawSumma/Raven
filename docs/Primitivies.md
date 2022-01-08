# Raven Primitives
---
## Primitives Types
Each primitive type in Raven alongside it's equivalent in C.
### Numeric 
#### Integers
##### Signed
* `int8`  ( `int8_t` )
* `int16` ( `int16_t` )
* `int32` ( `int32_t` )
* `int64` ( `int64_t` )
##### Unsigned
* `uint8`  ( `uint8_t` )
* `uint16` ( `uint16_t` )
* `uint32` ( `uint32_t` )
* `uint64` ( `uint64_t` )
#### Floats
* `float32` ( `float` )
* `float64` ( `double` )
### Boolean
* `bool` ( `bool` *in C++*)
### Pointers
* `&` character marks a type as a reference
  * `var ptr: &u8`
### Text
* `char` ( `char` )
* `string` ( `char*` )
---
## Interactions between primitivies
### Numeric ⟷ Numeric
Numeric primitives can interact with each other but a warning will be presented to the user to inform them that there is a mismatch in the types but if the result of the interaction is not of the same type as a variable a semantic error will be raised. During an interaction between two numeric data types the smaller of the two will be cast to the size of the larger.
#### Float ⟷ Integer
Floats and integers must be cast to either or the other before an interaction will be viable. A mismatch is considered a semantic error and will be a failing condition for a compilation.
### Numeric ⟷ Boolean
Numeric data types cannot interact with a boolean, there is no viable way to convert a boolean into a number or vice versa. Numbers interacting with relational operators (e.g. <, <= etc.) turn into booleans.
### Numeric ⟷ Pointers
Unsure if pointer arithmetic will be used in Raven.
### Numeric ⟷ Text
Integers, both signed and unsigned can be used in the indexing of strings, or the manipulation of characters.
### Booelan ⟷ Pointers
A boolean to pointer interaction results in a compile time error the two concepts are too far apart to justify behaviour, undefined or otherwise.
### Boolean ⟷ Text
As previous definitions for boolean interactions, boolean values are unable to interact with textual data and results in a compile time error.
### Pointer ⟷ Text
A pointer cannot directly interact with text such as an numeric dat type may index a string but a pointer to the textual data can be taken using the `&` operator.
