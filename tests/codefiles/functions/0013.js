const indexOf = (arr, val) => 
{
	for (let i in arr) {
		if (arr[i] === val)
			return i;
	}
	return -1;
}
x = indexOf([0,2,3,4,56], 3);
console.log("type=" + typeof x, "\nvalue="+x);


