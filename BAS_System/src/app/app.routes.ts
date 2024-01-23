import { Routes } from '@angular/router';
import { EmployeeComponent } from './employee/employee.component';
import { RegisterComponent } from './register/register.component';
import { HomeComponent } from './home/home.component';

export const routes: Routes = [
    {
        path: "",
        component: HomeComponent,
    },
    {
        path: "home",
        component: HomeComponent
    },
    {
        path: "employee-list",
        component: EmployeeComponent
    },
    {
        path: "register-employee",
        component: RegisterComponent
    }
];
