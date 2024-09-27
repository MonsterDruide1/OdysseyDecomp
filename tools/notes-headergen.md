For replacing `T const&` and `T const*` with `const T&*`:
SEARCH `(^|([\(,]))\s*([^\s]+) const([&*])`
REPLACE `$1const $3$4`
