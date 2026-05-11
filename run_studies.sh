BINARY="./sorting"
ITERATIONS=67
RESULTS_DIR="../results"

mkdir -p $RESULTS_DIR

echo "=== Starting all studies ==="
echo ""
echo "=== Study ALPHA ==="

ALPHA_SIZE=25000

echo "QuickSort - random, middle, right"
$BINARY --benchmark -a 5 -s 0 -t 0 -l $ALPHA_SIZE -n $ITERATIONS -r $RESULTS_DIR/study_alpha_quick.csv -d 0 -p 0
$BINARY --benchmark -a 5 -s 0 -t 0 -l $ALPHA_SIZE -n $ITERATIONS -r $RESULTS_DIR/study_alpha_quick.csv -d 0 -p 2
$BINARY --benchmark -a 5 -s 0 -t 0 -l $ALPHA_SIZE -n $ITERATIONS -r $RESULTS_DIR/study_alpha_quick.csv -d 0 -p 3

echo "ShellSort - different gap formulas..."
$BINARY --benchmark -a 6 -s 0 -t 0 -l $ALPHA_SIZE -n $ITERATIONS -r $RESULTS_DIR/study_alpha_shell.csv -d 0 -e 0
$BINARY --benchmark -a 6 -s 0 -t 0 -l $ALPHA_SIZE -n $ITERATIONS -r $RESULTS_DIR/study_alpha_shell.csv -d 0 -e 1

echo "Study ALPHA done!"
echo ""
echo "=== Study A ==="

SIZES=(10000 25000 50000 100000)
ALGORITHMS=(4 5 6)   # bucket, quick, shell
STRUCTURES=(0 1 2)   # array, singleList, doubleList

for size in "${SIZES[@]}"; do
    echo "Size: $size"
    for algo in "${ALGORITHMS[@]}"; do
        for struct in "${STRUCTURES[@]}"; do
            $BINARY --benchmark -a $algo -s $struct -t 0 -l $size -n $ITERATIONS -r $RESULTS_DIR/study_A.csv -d 0
        done
    done
done

echo "Study A done!"
echo ""
echo "=== Study B ==="

B_SIZE=25000
DISTRIBUTIONS=(0 1 2 3)  # random, ascending, ascending50, descending

for dist in "${DISTRIBUTIONS[@]}"; do
    echo "Distribution: $dist"
    for struct in "${STRUCTURES[@]}"; do
        $BINARY --benchmark -a 5 -s $struct -t 0 -l $B_SIZE -n $ITERATIONS -r $RESULTS_DIR/study_B.csv -d $dist -p 2
    done
done

echo "Study B done!"
echo ""
echo "=== Study C ==="

C_SIZE=25000
TYPES=(0 1 4 6)  # int, float, string, unsignedLong

for t in "${TYPES[@]}"; do
    echo "Type: $t"
    $BINARY --benchmark -a 5 -s 0 -t $t -l $C_SIZE -n $ITERATIONS -r $RESULTS_DIR/study_C.csv -d 0 -p 2
done

echo "Study C done!"
echo ""
echo "=== Study OMEGA ==="

OMEGA_SIZE=25000
OMEGA_STRUCTURES=(0 1 2 4 5)  # array, singleList, doubleList, stack, binaryTree

for struct in "${OMEGA_STRUCTURES[@]}"; do
    echo "Structure: $struct"
    $BINARY --benchmark -a 5 -s $struct -t 0 -l $OMEGA_SIZE -n $ITERATIONS -r $RESULTS_DIR/study_Omega.csv -d 0 -p 2
done

echo "Study OMEGA done!"
echo ""
echo "=== All studies completed! ==="
echo "Results saved in: $RESULTS_DIR/"
ls -la $RESULTS_DIR/