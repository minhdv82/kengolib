package algebra

import (
	"math/rand"
	"testing"
	"time"
)

func genVec(sz int) []uint64 {
	var rng = rand.NewSource(time.Now().UnixNano() + 82)
	var v []uint64
	for i := 0; i < sz; i++ {
		v = append(v, uint64(rng.Int63()))
	}
	return v
}
func TestMul(t *testing.T) {
	var u, v = genVec(1000), genVec(1000)
	var x, y = Bigint{u}, Bigint{v}
	var lhs, rhs = x.Mul(&y), y.Mul(&x)
	if !lhs.Compare(rhs) {
		t.Fatalf("Bigint mul is not commutative!\n")
	}
}
func TestAdd(t *testing.T) {
	var u, v = genVec(10000), genVec(10000)
	var x, y = Bigint{u}, Bigint{v}
	var lhs, rhs = x.Add(&y), y.Add(&x)
	if !lhs.Compare(rhs) {
		t.Fatalf("Bigint add is not commutative!\n")
	}
}
func TestDistributeLaw(t *testing.T) {
	var u, v = genVec(100), genVec(100)
	var x, y = Bigint{u}, Bigint{v}
	var xpy, xx, xy = x.Add(&y), x.Mul(&x), x.Mul(&y)
	var lhs, rhs = x.Mul(xpy), xx.Add(xy)
	if !lhs.Compare(rhs) {
		t.Fatalf("Bigint mul is not distributed with add!\n")
	}
}
