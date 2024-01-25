import { Component } from '@angular/core';
import {MatInputModule} from '@angular/material/input';

@Component({
  selector: 'app-register',
  standalone: true,
  imports: [MatInputModule],
  templateUrl: './register.component.html',
  styleUrl: './register.component.css'
})
export class RegisterComponent {

}
