harry = {age:15, name:"harry"};
sally = {age:14, name:"sally"};
harry.friends = [sally]
sally.friends = [harry]
ser = JSON.stringify(harry);
console.log("type=" + typeof ser + "\nvalue=" + ser);
// ERROR: CIRCULAR structure

