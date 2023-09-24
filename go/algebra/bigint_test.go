package algebra

import (
	"math/rand"
	"testing"
	"time"
)

func genVec(sz int) []uint64 {
	rng := rand.NewSource(time.Now().UnixNano() + 82)
	var v []uint64
	for i := 0; i < sz; i++ {
		v = append(v, uint64(rng.Int63()))
	}
	return v
}
func TestMul(t *testing.T) {
	u, v := genVec(1000), genVec(1000)
	x, y := Bigint{u}, Bigint{v}
	lhs, rhs := x.Mul(&y), y.Mul(&x)
	if !lhs.Compare(rhs) {
		t.Fatalf("Bigint mul is not commutative!\n")
	}
}
func TestAdd(t *testing.T) {
	u, v := genVec(1000), genVec(1000)
	x, y := Bigint{u}, Bigint{v}
	lhs, rhs := x.Add(&y), y.Add(&x)
	if !lhs.Compare(rhs) {
		t.Fatalf("Bigint add is not commutative!\n")
	}
}
func TestSub(t *testing.T) {
	u, v := genVec(1000), genVec(1000)
	x, y := Bigint{u}, Bigint{v}
	lhs, rhs := (x.Add(&y)).Sub(&x), &y
	if !lhs.Compare(rhs) {
		t.Fatalf("Bigint sub fails!\n")
	}
}
func TestDistributiveLaw(t *testing.T) {
	u, v := genVec(1000), genVec(1000)
	x, y := Bigint{u}, Bigint{v}
	xpy, xx, xy := x.Add(&y), x.Mul(&x), x.Mul(&y)
	lhs, rhs := x.Mul(xpy), xx.Add(xy)
	if !lhs.Compare(rhs) {
		t.Fatalf("Bigint mul is not distributive over add!\n")
	}
}
func TestDivMod(t *testing.T) {
	u, v := genVec(1500), genVec(1500)
	x, y := Bigint{u}, Bigint{v}
	div, mod := x.DivMod(&y)
	lhs, rhs := mod.Add(div.Mul(&y)), &x
	if !lhs.Compare(rhs) {
		t.Fatalf("Bigint divmod fails!\n")
	}
}
func TestGcd(t *testing.T) {
	u, v := genVec(1500), genVec(1500)
	x, y := Bigint{u}, Bigint{v}
	g := x.Gcd(y)
	lhs, rhs := x.Modulo(g), y.Modulo(g)
	if !lhs.Zero() || !rhs.Zero() {
		t.Fatalf("Bigint gcd fails!\n")
	}
}
