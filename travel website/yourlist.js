console.log("hello");

//Declare global variables
const input = document.getElementById("new-wish-input");
const wishList = document.getElementById("wish-list");

// Add input to list when "Add" button clicked
// Add event listener to Add button
const elem = document.getElementById("add-wish-button");
if(elem == null){
  console.log("NULL aahe");
}
elem.addEventListener("click", addWish);
// Define addWish function
function addWish() {
  // Create list item when a new wish is entered
  if (input.value !== "") {
    const li = document.createElement("li");
    li.className = "wish-item";

    //Include a checkbox in the list item and add an event listener
    const checkBox = document.createElement("input");
    checkBox.type = "checkbox";
    checkBox.className = "check-box";
    checkBox.addEventListener("change", toggleWishCompletion);

    //Include the input value as the text of the list item
    const wishText = document.createElement("span");
    wishText.textContent = input.value;

    //Include a button for deleting the list item and add an event listener
    const deleteButton = document.createElement("span");
    deleteButton.textContent = "x";
    deleteButton.className = "delete-button";
    deleteButton.addEventListener("click", deleteWish);

    //Append each component defined above to the list item
    li.appendChild(checkBox);
    li.appendChild(wishText);
    li.appendChild(deleteButton);

    //Append the list item to the list
    wishList.appendChild(li);

    //Clear the input value, resets back to placeholder value
    input.value = "";
  }
}

//Toggle checkbox status
function toggleWishCompletion() {
  const wishItem = this.parentElement;
  wishItem.classList.toggle("completed");
}

//Delete specific wish from list
function deleteWish() {
  const wishItem = this.parentElement;
  wishItem.remove();
}

// Remove all items from list when "Reset" button clicked
// Add event listener to Reset button
document.getElementById("reset-button").addEventListener("click", resetList);

// Define reset function
function resetList() {
  //Remove all items list items from ul
  wishList.innerHTML = "";
  //Clear the input value, resets back to placeholder value
  input.value = "";
}
