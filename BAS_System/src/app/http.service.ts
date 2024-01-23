import { HttpClient } from '@angular/common/http';
import { Injectable, inject } from '@angular/core';
import { IEmployee } from './employee/interfaces/employee';

@Injectable({
  providedIn: 'root'
})
export class HttpService {
  apiUrl = "http:localhost:4200";
  http = inject(HttpClient);
  constructor() {  
  }
  getAll(){
    return this.http.get<IEmployee[]>("/assets/data.json");
  }
}
