class Employee:
    def __init__(self, name, age, gender, role, salary):
        self.name = name
        self.age = age
        self.gender = gender
        self.role = role
        self.salary = salary

class Recruiter:
    def __init__(self):
        self.employees = []

    def add_employee(self, employee):
        self.employees.append(employee)

    def remove_employee(self, employee):
        self.employees.remove(employee)

    def search_employee(self, name):
        for employee in self.employees:
            if employee.name == name:
                return employee
        return None

    def display_employees(self):
        for employee in self.employees:
            print(f"Name: {employee.name}, Age: {employee.age}, Gender: {employee.gender}, Role: {employee.role}, Salary: {employee.salary}")
# create employees
john = Employee("John", 25, "Male", "Software Engineer", 50000)
jane = Employee("Jane", 30, "Female", "Project Manager", 70000)

# create recruiter
recruiter = Recruiter()

# add employees
recruiter.add_employee(john)
recruiter.add_employee(jane)

# display employees
recruiter.display_employees()

# remove employee
recruiter.remove_employee(john)

# search employee
employee = recruiter.search_employee("Jane")
# employee = recruiter.search_employee("Jany")

if employee:
    print(f"Found employee: {employee.name}")
else:
    print("Employee not found")

