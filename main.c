// ============================================================================
// 👤 PART 1: General Architect
// Responsibility: Libraries, Structs, and Function Prototypes.
// ============================================================================

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CURRENT_YEAR 2025

// Car Structure Definition
typedef struct {
  int id;
  char model[50];
  char owner[50];
  int year;
  int isRented; // 0 = Available, 1 = Rented
  double price; // Original Purchase Price
} Car;

// User Structure Definition
typedef struct {
  char username[50];
  char password[50];
} User;

// Function Prototypes
void clearInput();
void saveUser(User u);
int checkUser(char *username, char *password);
void authSystem(char *loggedInUser);
void saveToFile(Car *cars, int count);
Car *loadFromFile(int *count);
Car *addCar(Car *cars, int *count, const char *currentUser);
Car *deleteCar(Car *cars, int *count, const char *currentUser);
void logActivity(const char *username, const char *action);
void rentalSystem(Car *cars, int count);
void calculateDepreciation(Car *cars, int count);
void displayCars(Car *cars, int count);
void searchCar(Car *cars, int count);

// ============================================================================
// 👤 PART 2: Security Module
// Responsibility: Login system, Account creation, and Input validation.
// ============================================================================

void clearInput() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF);
}

void saveUser(User u) {
  FILE *file = fopen("users.txt", "a");
  if (file) {
    fprintf(file, "%s %s\n", u.username, u.password);
    fclose(file);
  }
}

int checkUser(char *username, char *password) {
  FILE *file = fopen("users.txt", "r");
  if (!file) return 0;

  char u[50], p[50];
  while (fscanf(file, "%s %s", u, p) != EOF) {
    if (strcmp(username, u) == 0 && strcmp(password, p) == 0) {
      fclose(file);
      return 1;
    }
  }
  fclose(file);
  return 0;
}

void authSystem(char *loggedInUser) {
  int choice;
  User u;
  int loggedIn = 0;

  while (!loggedIn) {
    printf("\n=== 🔒 SYSTEM SECURITY INTERFACE ===\n");
    printf("1. Login\n");
    printf("2. Sign Up\n");
    printf("3. Exit System\n");
    printf("Choose: ");

    if (scanf("%d", &choice) != 1) {
      clearInput();
      choice = 0;
    }

    if (choice == 1) {
      printf("Username: ");
      scanf("%s", u.username);
      printf("Password: ");
      scanf("%s", u.password);
      if (checkUser(u.username, u.password)) {
        printf("\n✅ Access Granted! Welcome, %s.\n", u.username);
        loggedIn = 1;
        strcpy(loggedInUser, u.username);
      } else {
        printf("\n❌ Access Denied! Invalid credentials.\n");
      }
    } else if (choice == 2) {
      printf("Enter New Username: ");
      scanf("%s", u.username);
      printf("Enter New Password: ");
      scanf("%s", u.password);
      saveUser(u);
      printf("\n✅ Account Created! You can now login.\n");
    } else if (choice == 3) {
      exit(0);
    }
  }
}

// ============================================================================
// 👤 PART 3: Database & Persistence Layer
// Responsibility: File I/O operations (cars.txt).
// ============================================================================

void saveToFile(Car *cars, int count) {
  FILE *file = fopen("cars.txt", "w");
  if (!file) {
    printf("Error: File write access denied.\n");
    return;
  }
  for (int i = 0; i < count; i++) {
    fprintf(file, "%d\n%s\n%s\n%d\n%d\n%.2f\n", cars[i].id, cars[i].model,
            cars[i].owner, cars[i].year, cars[i].isRented, cars[i].price);
  }
  fclose(file);
}

Car *loadFromFile(int *count) {
  FILE *file = fopen("cars.txt", "r");
  if (!file) return NULL;

  Car *cars = NULL;
  int tempCount = 0;
  Car temp;

  while (fscanf(file, "%d\n", &temp.id) != EOF) {
    fscanf(file, " %[^\n]\n", temp.model);
    fscanf(file, " %[^\n]\n", temp.owner);
    fscanf(file, "%d\n", &temp.year);
    fscanf(file, "%d\n", &temp.isRented);
    fscanf(file, "%lf\n", &temp.price);

    tempCount++;
    cars = (Car *)realloc(cars, tempCount * sizeof(Car));
    cars[tempCount - 1] = temp;
  }
  fclose(file);
  *count = tempCount;
  return cars;
}

// ============================================================================
// 👤 PART 4: Core Operations Manager
// Responsibility: Management of car records and memory allocation.
// ============================================================================

Car *addCar(Car *cars, int *count, const char *currentUser) {
  *count += 1;
  Car *new_cars = (Car *)realloc(cars, (*count) * sizeof(Car));
  if (!new_cars) {
    printf("Memory Critical Error!\n");
    exit(1);
  }
  cars = new_cars;

  printf("\n--- Add New Vehicle Entry ---\n");
  printf("Enter ID: ");
  scanf("%d", &cars[*count - 1].id);
  printf("Enter Model: ");
  scanf(" %[^\n]", cars[*count - 1].model);
  printf("Enter Owner: ");
  scanf(" %[^\n]", cars[*count - 1].owner);
  printf("Enter Year: ");
  scanf("%d", &cars[*count - 1].year);
  printf("Enter Original Price ($): ");
  if (scanf("%lf", &cars[*count - 1].price) != 1) {
    cars[*count - 1].price = 0;
  }
  clearInput();

  cars[*count - 1].isRented = 0;
  saveToFile(cars, *count);

  char action[100];
  sprintf(action, "added car: %s", cars[*count - 1].model);
  logActivity(currentUser, action);

  printf("✅ Entry logged successfully.\n");
  return cars;
}

Car *deleteCar(Car *cars, int *count, const char *currentUser) {
  if (*count == 0) return cars;
  int id, found = 0;
  printf("Enter ID to remove: ");
  scanf("%d", &id);
  clearInput();

  for (int i = 0; i < *count; i++) {
    if (cars[i].id == id) {
      found = 1;
      for (int j = i; j < *count - 1; j++) cars[j] = cars[j + 1];
      *count -= 1;
      if (*count > 0)
        cars = (Car *)realloc(cars, (*count) * sizeof(Car));
      else {
        free(cars);
        cars = NULL;
      }
      saveToFile(cars, *count);
      char action[50];
      sprintf(action, "deleted ID: %d", id);
      logActivity(currentUser, action);
      printf("✅ Entry removed.\n");
      break;
    }
  }
  if (!found) printf("❌ Record not found.\n");
  return cars;
}

// ============================================================================
// 👤 PART 5: UI & Specialized Features
// Responsibility: Rental logic, Depreciation engine, and View generation.
// ============================================================================

void rentalSystem(Car *cars, int count) {
  if (count == 0) return;
  int choice, id, found = 0;
  printf("\n--- Rental Management Module ---\n");
  printf("1. Check-out (Rent)\n");
  printf("2. Check-in (Return)\n");
  scanf("%d", &choice);
  printf("Enter Car ID: ");
  scanf("%d", &id);
  clearInput();

  for (int i = 0; i < count; i++) {
    if (cars[i].id == id) {
      found = 1;
      if (choice == 1) {
        if (cars[i].isRented) printf("⚠ Vehicle currently unavailable.\n");
        else { cars[i].isRented = 1; printf("✅ Check-out complete.\n"); }
      } else {
        if (!cars[i].isRented) printf("⚠ Vehicle is already in stock.\n");
        else { cars[i].isRented = 0; printf("✅ Check-in complete.\n"); }
      }
      saveToFile(cars, count);
      break;
    }
  }
  if (!found) printf("❌ Invalid ID.\n");
}

void calculateDepreciation(Car *cars, int count) {
  int id, found = 0;
  printf("Enter ID for Valuation: ");
  scanf("%d", &id);
  clearInput();

  for (int i = 0; i < count; i++) {
    if (cars[i].id == id) {
      found = 1;
      int age = CURRENT_YEAR - cars[i].year;
      if (age < 0) age = 0;
      double currentValue = cars[i].price * pow(0.95, age);

      printf("\n--- 📉 ASSET VALUATION REPORT ---\n");
      printf("Asset: %s (%d)\n", cars[i].model, cars[i].year);
      printf("Acquisition Cost: $%.2f\n", cars[i].price);
      printf("Age: %d years\n", age);
      printf("Current Market Value: $%.2f\n", currentValue);
      break;
    }
  }
  if (!found) printf("❌ ID not found.\n");
}

void displayCars(Car *cars, int count) {
  if (count == 0) { printf("Database is empty.\n"); return; }
  printf("\n%-5s | %-15s | %-15s | %-6s | %-10s | %-10s\n", "ID", "Model", "Owner", "Year", "Status", "Price($)");
  printf("----------------------------------------------------------------------\n");
  for (int i = 0; i < count; i++) {
    printf("%-5d | %-15s | %-15s | %-6d | %-10s | %-10.2f\n", cars[i].id,
           cars[i].model, cars[i].owner, cars[i].year,
           cars[i].isRented ? "⛔ RENTED" : "✅ FREE", cars[i].price);
  }
}

void searchCar(Car *cars, int count) {
  int id;
  printf("Search by ID: ");
  scanf("%d", &id);
  clearInput();
  for (int i = 0; i < count; i++) {
    if (cars[i].id == id) {
      printf("\nFOUND: %s (%d) - Owner: %s - Status: %s\n", cars[i].model,
             cars[i].year, cars[i].owner, cars[i].isRented ? "Rented" : "Available");
      return;
    }
  }
  printf("❌ No record matches this ID.\n");
}

// ============================================================================
// 👤 PART 6: System Activity Logger
// Responsibility: Audit trailing and activity tracking.
// ============================================================================

void logActivity(const char *username, const char *action) {
  FILE *file = fopen("activity_log.txt", "a");
  if (!file) return;

  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  char timeStr[64];
  strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %I:%M %p", &tm);

  fprintf(file, "[%s] User: %s - Action: %s\n", timeStr, username, action);
  fclose(file);
}

// ============================================================================
// 👤 PART 7: Application Controller
// Responsibility: Main execution loop and orchestration.
// ============================================================================

int main() {
  char currentUser[50] = "";
  authSystem(currentUser);

  int count = 0;
  Car *cars = loadFromFile(&count);
  int choice;

  do {
    printf("\n=== 🚗 FLEET MANAGEMENT SYSTEM CORE ===\n");
    printf("1. Add Vehicle\n");
    printf("2. Delete Vehicle\n");
    printf("3. View Fleet Inventory\n");
    printf("4. Search Records\n");
    printf("5. Rental Transaction\n");
    printf("6. Financial Valuation\n");
    printf("7. Secure Exit\n");
    printf("Action > ");

    if (scanf("%d", &choice) != 1) {
      clearInput();
      choice = 0;
    } else {
      clearInput();
    }

    switch (choice) {
      case 1: cars = addCar(cars, &count, currentUser); break;
      case 2: cars = deleteCar(cars, &count, currentUser); break;
      case 3: displayCars(cars, count); break;
      case 4: searchCar(cars, count); break;
      case 5: rentalSystem(cars, count); break;
      case 6: calculateDepreciation(cars, count); break;
      case 7: printf("Syncing database and closing...\n"); break;
      default: printf("Invalid Selection.\n");
    }
  } while (choice != 7);

  free(cars);
  return 0;
}
