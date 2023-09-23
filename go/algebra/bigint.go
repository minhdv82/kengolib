package algebra

import (
	"fmt"
	"math/bits"
)

type Bigint struct {
	value []uint64
}

func (x *Bigint) Print() {
	fmt.Printf("%v", x.value[0])
	for i := 1; i < x.size(); i++ {
		fmt.Printf("-%v", x.value[i])
	}
	fmt.Println("")
}

func (x *Bigint) size() int {
	return len(x.value)
}

var BI0 Bigint = Bigint{[]uint64{0}}
var BI1 Bigint = Bigint{[]uint64{1}}

func add(i int, x []uint64, y uint64) (carry uint64) {
	x[i], carry = bits.Add64(x[i], y, 0)
	return
}

func sub(i int, x []uint64, y uint64) (carry uint64) {
	x[i], carry = bits.Sub64(x[i], y, 0)
	return
}

func mul(i int, x []uint64, y uint64) (carry uint64) {
	carry, x[i] = bits.Mul64(x[i], y)
	return
}

func popFront(x []uint64) []uint64 {
	return x[1:]
}

func popBack(x []uint64) []uint64 {
	return x[:len(x)-1]
}

func pushFront(val uint64, x []uint64) []uint64 {
	return append([]uint64{val}, x...)
}

func pushBack(val uint64, x []uint64) []uint64 {
	return append(x, val)
}

func canonize(x []uint64) []uint64 {
	for len(x) > 1 && x[0] == 0 {
		x = popFront(x)
	}
	return x
}

func (lhs Bigint) Compare(rhs *Bigint) bool {
	if lhs.size() != rhs.size() {
		return false
	}
	for i := 0; i < lhs.size(); i++ {
		if lhs.value[i] != rhs.value[i] {
			return false
		}
	}
	return true
}

func (lhs Bigint) Order(rhs *Bigint) bool {
	if lhs.size() > rhs.size() {
		return true
	} else if lhs.size() < rhs.size() {
		return false
	}
	for i := 0; i < lhs.size(); i++ {
		if lhs.value[i] > rhs.value[i] {
			return true
		} else if lhs.value[i] < rhs.value[i] {
			return false
		}
	}
	return true
}

func (x Bigint) Zero() bool {
	return x.size() == 1 && x.value[0] == 0
}

func (x Bigint) Unit() bool {
	return x.size() == 1 && x.value[0] == 1
}

func (lhs Bigint) Modulo(rhs *Bigint) Bigint {
	if rhs.Zero() {
		return BI0
	}
	return Bigint{}
}

func (x Bigint) And(val uint64) uint64 {
	return x.value[x.size()-1] & val
}

func (lhs Bigint) Add(rhs *Bigint) *Bigint {
	var big, sml []uint64
	if lhs.size() > rhs.size() {
		big = lhs.value
		sml = rhs.value
	} else {
		big = rhs.value
		sml = lhs.value
	}
	value := make([]uint64, len(big))
	_ = copy(value, big)
	var c uint64 = 0
	gap := len(big) - len(sml)
	i := len(sml) - 1
	for i >= 0 {
		cc := add(i+gap, value, sml[i])
		c = add(i+gap, value, c)
		c += cc
		i--
	}
	i = gap - 1
	for c > 0 {
		if i < 0 {
			value = pushFront(c, value)
			break
		}
		c = add(i, value, c)
		i--
	}
	return &Bigint{value}
}

func (lhs Bigint) Sub(rhs *Bigint) *Bigint {
	if lhs.Compare(rhs) || !lhs.Order(rhs) {
		return &Bigint{[]uint64{0}}
	}
	var big, sml []uint64 = lhs.value, rhs.value

	value := make([]uint64, len(big))
	copy(value, big)
	var c uint64 = 0
	gap := len(big) - len(sml)
	i := len(sml) - 1
	for i >= 0 {
		cc := sub(i+gap, value, sml[i])
		c = sub(i, value, c)
		c += cc
		i--
	}
	i = len(big) - len(sml) - 1
	for c > 0 {
		c = sub(i, value, c)
		i--
	}
	value = canonize(value)
	return &Bigint{value}
}

func (x Bigint) MulUint(val uint64) *Bigint {
	if val == 0 || x.Zero() {
		return &BI0
	}
	value := make([]uint64, x.size())
	copy(value, x.value)
	var c uint64 = 0
	for i := len(value) - 1; i >= 0; i-- {
		var cc = mul(i, value, val)
		c = add(i, value, c)
		c += cc
	}
	if c > 0 {
		value = pushFront(c, value)
	}
	return &Bigint{value}
}

func (x Bigint) RightShift(n int) *Bigint {
	if n < 1 || x.Zero() {
		return &x
	}
	var value = make([]uint64, len(x.value))
	copy(value, x.value)
	for n >= 64 {
		value = popBack(value)
		if len(value) < 1 {
			return &BI0
		}
		n -= 64
		if n == 0 {
			return &Bigint{value}
		}
	}
	var c uint64 = 0
	for i := 0; i < len(value); i++ {
		var val uint64 = (value[i] >> n) | c
		c = (value[i]) << (64 - n)
		value[i] = val
	}
	value = canonize(value)
	return &Bigint{value}
}

func (x Bigint) LeftShift(n int) *Bigint {
	if n < 1 || x.Zero() {
		return &x
	}
	value := make([]uint64, len(x.value))
	copy(value, x.value)
	for n >= 64 {
		value = pushBack(0, value)
		n -= 64
		if n == 0 {
			return &Bigint{value}
		}
	}
	var c uint64 = 0
	for i := len(value) - 1; i >= 0; i-- {
		val := (value[i] << n) | c
		c = (value[i]) >> (64 - n)
		value[i] = val
	}
	if c > 0 {
		value = pushFront(c, value)
	}
	return &Bigint{value}
}

func (lhs Bigint) Mul(rhs *Bigint) *Bigint {
	res := lhs.MulUint(rhs.value[0])
	for i := 1; i < rhs.size(); i++ {
		res = res.LeftShift(64)
		val := lhs.MulUint(rhs.value[i])
		res = val.Add(res)
	}
	return res
}
