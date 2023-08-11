function indexOf(arr, value) {
	for (let i in arr) {
		if (arr[i] === value)
			return i;
	}
	return -1;
}
let x = indexOf([1,2,3,4,5,6,7,8,9], 7);

console.log("type=" + typeof x, "\nvalue="+x);


