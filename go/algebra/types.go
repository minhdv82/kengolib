package algebra

// == operator in C
type Comparable interface {
	Compare(Comparable) bool
}

// > operator in C
type Orderable interface {
	Order(Orderable) bool
}

type Zeroable interface {
	Zero() bool
}

type Oneable interface {
	Unit() bool
}

type Moduloable interface {
	Zeroable
	Modulo(Moduloable) Moduloable
}

type Dividable interface {
	Zeroable
	Oneable
	Divide(Dividable) Dividable
}

// gcd is defined for moduloable type
func Gcd(x Moduloable, y Moduloable) Moduloable {
	if y.Zero() {
		return x
	}
	return Gcd(y, x.Modulo(y))
}
