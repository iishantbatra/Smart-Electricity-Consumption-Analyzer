#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_APPLIANCES 100
#define DATA_FILE "appliances.dat"
#define DAYS_IN_MONTH 30

#define RATE_1 3.00
#define RATE_2 5.00
#define RATE_3 7.00
#define RATE_4 8.00

typedef struct {
    char name[50];
    double powerWatts;
    double hoursPerDay;
    int quantity;
} Appliance;

Appliance appliances[MAX_APPLIANCES];
int applianceCount = 0;

void readLine(const char *prompt, char *buffer, int size) {
    int ch;
    printf("%s", prompt);

    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }

    buffer[strcspn(buffer, "\n")] = '\0';

    if (strchr(buffer, '\0') == buffer + size - 1) {
        while ((ch = getchar()) != '\n' && ch != EOF) {}
    }
}

double readDouble(const char *prompt, double minValue, double maxValue) {
    char line[100];
    char *end;
    double value;

    while (1) {
        readLine(prompt, line, sizeof(line));
        value = strtod(line, &end);

        while (*end == ' ' || *end == '\t') {
            end++;
        }

        if (line[0] != '\0' && end != line && *end == '\0' && value >= minValue && value <= maxValue) {
            return value;
        }

        printf("Invalid input. Enter a value from %.2f to %.2f.\n", minValue, maxValue);
    }
}

int readInt(const char *prompt, int minValue, int maxValue) {
    char line[100];
    char *end;
    long value;

    while (1) {
        readLine(prompt, line, sizeof(line));
        value = strtol(line, &end, 10);

        while (*end == ' ' || *end == '\t') {
            end++;
        }

        if (line[0] != '\0' && end != line && *end == '\0' && value >= minValue && value <= maxValue) {
            return (int)value;
        }

        printf("Invalid input. Enter a whole number from %d to %d.\n", minValue, maxValue);
    }
}

void saveData(void) {
    FILE *fp = fopen(DATA_FILE, "wb");
    if (fp == NULL) {
        printf("Warning: Could not save appliance records.\n");
        return;
    }

    fwrite(&applianceCount, sizeof(int), 1, fp);
    fwrite(appliances, sizeof(Appliance), applianceCount, fp);
    fclose(fp);
}

void loadData(void) {
    FILE *fp = fopen(DATA_FILE, "rb");
    int count;

    if (fp == NULL) return;

    if (fread(&count, sizeof(int), 1, fp) == 1 && count >= 0 && count <= MAX_APPLIANCES) {
        if (fread(appliances, sizeof(Appliance), count, fp) == (size_t)count) {
            applianceCount = count;
        }
    }
    fclose(fp);
}

double dailyUnits(const Appliance *a) {
    return (a->powerWatts * a->hoursPerDay * a->quantity) / 1000.0;
}

double monthlyUnits(const Appliance *a) {
    return dailyUnits(a) * DAYS_IN_MONTH;
}

double totalDailyUnits(void) {
    double total = 0.0;
    for (int i = 0; i < applianceCount; i++) {
        total += dailyUnits(&appliances[i]);
    }
    return total;
}

double totalMonthlyUnits(void) {
    return totalDailyUnits() * DAYS_IN_MONTH;
}

double calculateBill(double units) {
    double bill = 0.0;

    if (units <= 100.0) {
        bill = units * RATE_1;
    } else if (units <= 200.0) {
        bill = (100.0 * RATE_1) + ((units - 100.0) * RATE_2);
    } else if (units <= 500.0) {
        bill = (100.0 * RATE_1) + (100.0 * RATE_2) + ((units - 200.0) * RATE_3);
    } else {
        bill = (100.0 * RATE_1) + (100.0 * RATE_2) + (300.0 * RATE_3) + ((units - 500.0) * RATE_4);
    }

    return bill;
}

void addAppliance(void) {
    if (applianceCount >= MAX_APPLIANCES) {
        printf("\nAppliance limit reached (Maximum: %d).\n", MAX_APPLIANCES);
        return;
    }

    Appliance a;
    printf("\n--- Add Appliance ---\n");
    readLine("Enter appliance name: ", a.name, sizeof(a.name));

    if (strlen(a.name) == 0) {
        printf("Name cannot be empty.\n");
        return;
    }

    a.powerWatts = readDouble("Power rating (watts): ", 1.0, 100000.0);
    a.hoursPerDay = readDouble("Average daily usage (hours): ", 0.0, 24.0);
    a.quantity = readInt("Quantity: ", 1, 100);

    appliances[applianceCount++] = a;
    saveData();
    printf("\nAppliance added successfully.\n");
}

void viewAppliances(void) {
    printf("\n--- Appliance List ---\n");
    if (applianceCount == 0) {
        printf("No appliances added yet.\n");
        return;
    }

    printf("%-4s %-22s %10s %10s %8s %12s\n", "No.", "Appliance", "Watts", "Hours/day", "Quantity", "Units/day");
    printf("--------------------------------------------------------------------------\n");

    for (int i = 0; i < applianceCount; i++) {
        printf("%-4d %-22.22s %10.2f %10.2f %8d %12.2f\n",
               i + 1, appliances[i].name, appliances[i].powerWatts,
               appliances[i].hoursPerDay, appliances[i].quantity,
               dailyUnits(&appliances[i]));
    }
}

void deleteAppliance(void) {
    if (applianceCount == 0) {
        printf("\nNo appliances to delete.\n");
        return;
    }

    viewAppliances();
    int number = readInt("\nEnter appliance number to delete: ", 1, applianceCount);

    for (int i = number - 1; i < applianceCount - 1; i++) {
        appliances[i] = appliances[i + 1];
    }

    applianceCount--;
    saveData();
    printf("Appliance deleted successfully.\n");
}

void showConsumption(void) {
    if (applianceCount == 0) {
        printf("\nAdd appliances first to calculate consumption.\n");
        return;
    }

    printf("\n--- Energy Consumption ---\n");
    printf("%-22s %12s %14s\n", "Appliance", "Daily (kWh)", "Monthly (kWh)");
    printf("---------------------------------------------------\n");

    for (int i = 0; i < applianceCount; i++) {
        printf("%-22.22s %12.2f %14.2f\n",
               appliances[i].name, dailyUnits(&appliances[i]),
               monthlyUnits(&appliances[i]));
    }

    printf("---------------------------------------------------\n");
    printf("%-22s %12.2f %14.2f\n", "TOTAL", totalDailyUnits(), totalMonthlyUnits());
}

void estimateBill(void) {
    if (applianceCount == 0) {
        printf("\nAdd appliances first to estimate the bill.\n");
        return;
    }

    double units = totalMonthlyUnits();
    double bill = calculateBill(units);

    printf("\n--- Monthly Bill Estimate ---\n");
    printf("Estimated Monthly Consumption : %.2f units\n", units);
    printf("Estimated Energy Charges      : Rs. %.2f\n", bill);
}

void rankAppliances(void) {
    if (applianceCount == 0) {
        printf("\nNo appliance data available.\n");
        return;
    }

    int order[MAX_APPLIANCES];
    for (int i = 0; i < applianceCount; i++) order[i] = i;

    for (int i = 0; i < applianceCount - 1; i++) {
        for (int j = i + 1; j < applianceCount; j++) {
            if (monthlyUnits(&appliances[order[j]]) > monthlyUnits(&appliances[order[i]])) {
                int temp = order[i];
                order[i] = order[j];
                order[j] = temp;
            }
        }
    }

    printf("\n--- Appliance Consumption Ranking ---\n");
    for (int i = 0; i < applianceCount; i++) {
        int index = order[i];
        printf("%d. %-22.22s %.2f units/month\n", i + 1, appliances[index].name, monthlyUnits(&appliances[index]));
    }
}

void detectWastage(void) {
    double total = totalMonthlyUnits();
    if (applianceCount == 0 || total <= 0.0) {
        printf("\nNo consumption data available.\n");
        return;
    }

    printf("\n--- High Consumption Analysis ---\n");
    int found = 0;

    for (int i = 0; i < applianceCount; i++) {
        double share = (monthlyUnits(&appliances[i]) / total) * 100.0;
        if (share >= 25.0) {
            printf("ALERT: %s consumes %.1f%% of total electricity.\n", appliances[i].name, share);
            found = 1;
        }
    }

    if (!found) {
        printf("No single appliance accounts for 25%% or more of total usage.\n");
    }
}

void energySavingSuggestions(void) {
    if (applianceCount == 0) {
        printf("\nAdd appliances first to receive suggestions.\n");
        return;
    }

    printf("\n--- Energy-Saving Suggestions ---\n");
    for (int i = 0; i < applianceCount; i++) {
        double saving = monthlyUnits(&appliances[i]) * 0.10;
        printf("\n%s:\n", appliances[i].name);
        if (appliances[i].hoursPerDay >= 8.0) {
            printf("- Consider reducing daily operational hours.\n");
        } else {
            printf("- Ensure it is switched off when not in use.\n");
        }
        printf("- A 10%% usage reduction saves ~%.2f units/month.\n", saving);
    }
}

void generateReport(void) {
    if (applianceCount == 0) {
        printf("\nNo data available.\n");
        return;
    }

    double totalUnits = totalMonthlyUnits();
    double totalBill = calculateBill(totalUnits);

    printf("\n============================================================\n");
    printf("             SMART ELECTRICITY ANALYSIS REPORT              \n");
    printf("============================================================\n");
    printf("Total Appliances            : %d\n", applianceCount);
    printf("Daily Usage                 : %.2f units\n", totalDailyUnits());
    printf("Monthly Usage               : %.2f units\n", totalUnits);
    printf("Estimated Bill              : Rs. %.2f\n", totalBill);
    printf("------------------------------------------------------------\n");
    printf("%-22s %12s %12s\n", "Appliance", "Units/month", "Share (%)");
    printf("------------------------------------------------------------\n");

    for (int i = 0; i < applianceCount; i++) {
        double usage = monthlyUnits(&appliances[i]);
        double share = (totalUnits > 0.0) ? (usage / totalUnits) * 100.0 : 0.0;
        printf("%-22.22s %12.2f %11.1f%%\n", appliances[i].name, usage, share);
    }
    printf("============================================================\n");
}

void displayMenu(void) {
    printf("\n===============================================\n");
    printf("       SMART ELECTRICITY CONSUMPTION ANALYZER  \n");
    printf("===============================================\n");
    printf("1. Add Appliance\n");
    printf("2. View Appliances\n");
    printf("3. Delete Appliance\n");
    printf("4. Show Consumption\n");
    printf("5. Estimate Monthly Bill\n");
    printf("6. Rank Appliances\n");
    printf("7. Detect High Consumption\n");
    printf("8. Energy-Saving Suggestions\n");
    printf("9. Generate Report\n");
    printf("0. Save and Exit\n");
    printf("-----------------------------------------------\n");
}

int main(void) {
    int choice;
    loadData();

    printf("\nWelcome to Smart Electricity Consumption Analyzer!\n");
    printf("%d record(s) loaded.\n", applianceCount);

    do {
        displayMenu();
        choice = readInt("Enter your choice: ", 0, 9);

        switch (choice) {
            case 1: addAppliance(); break;
            case 2: viewAppliances(); break;
            case 3: deleteAppliance(); break;
            case 4: showConsumption(); break;
            case 5: estimateBill(); break;
            case 6: rankAppliances(); break;
            case 7: detectWastage(); break;
            case 8: energySavingSuggestions(); break;
            case 9: generateReport(); break;
            case 0:
                saveData();
                printf("\nData saved. Exiting program...\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 0);

    return 0;
}
