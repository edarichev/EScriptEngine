let x;
function myFn(arg1)
{
	x = arguments;
}

myFn(1,2,3, "hello");

console.log("type=" + typeof x, "\nvalue=", x);


