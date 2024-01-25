import { Component, inject } from '@angular/core';
import { EmployeeInterface } from './interfaces/employee';
import { HttpService } from '../http.service';
import {MatTableDataSource, MatTableModule} from '@angular/material/table';
import {MatInputModule} from '@angular/material/input';
import {MatFormFieldModule} from '@angular/material/form-field';
@Component({
  selector: 'app-employee',
  standalone: true,
  imports: [MatTableModule, MatInputModule, MatFormFieldModule],
  templateUrl: './employee.component.html',
  styleUrl: './employee.component.css'
})
export class EmployeeComponent {
  employeelist: EmployeeInterface[] = [];
  httpService=inject(HttpService)
  displayedColumns: string[]= ['id', 'year', 'firstName', 'lastName', 'email', 'arrivedTime', 'delayedTime'];
  dataSource = new MatTableDataSource(this.employeelist);
  ngOnInit(){
    this.httpService.getAll().subscribe(result=>{
      this.employeelist = result;
    })
  }
}

