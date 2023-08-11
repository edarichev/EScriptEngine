function fn() {}

let a = fn;
let b = fn;

x = a instanceof Function;

console.log("type=" + typeof x, "\nvalue="+x);


