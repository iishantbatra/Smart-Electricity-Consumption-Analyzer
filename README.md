# Smart Electricity Consumption Analyzer

## Project Overview
The Smart Electricity Consumption Analyzer is a menu-driven application developed in the C programming language. It helps users estimate household electricity consumption, calculate an illustrative monthly electricity bill, identify high-consumption appliances, and view energy-saving suggestions.

This project is designed for an LTPS (Logic and Thinking through Problem Solving) course and demonstrates problem-solving using fundamental and intermediate C programming concepts.

## Objectives
- Record household appliances and their power ratings.
- Calculate daily and monthly electricity consumption.
- Estimate electricity charges using progressive sample tariff slabs.
- Rank appliances according to monthly energy consumption.
- Flag appliances that account for a large share of total consumption.
- Provide basic energy-saving suggestions.
- Save appliance records so they can be loaded in the next run.

## Features
1. **Add Appliance** — enter an appliance name, power rating in watts, average daily usage hours, and quantity.
2. **View Appliances** — display all saved appliance records.
3. **Delete Appliance** — remove an appliance record.
4. **Calculate Consumption** — calculate daily and monthly energy use.
5. **Estimate Bill** — calculate an estimated energy charge using the sample tariff slabs.
6. **Consumption Ranking** — rank appliances from highest to lowest monthly consumption.
7. **High Consumption Detection** — flag an appliance if it accounts for at least 25% of total recorded consumption.
8. **Energy-Saving Suggestions** — provide simple usage recommendations and an illustrative 10% saving estimate.
9. **Generate Report** — display a summary of consumption, estimated charges, and each appliance's share.

## Formulae Used

### Daily electricity consumption
`Daily Units (kWh) = Power (W) × Usage Hours per Day × Quantity ÷ 1000`

### Monthly electricity consumption
`Monthly Units (kWh) = Daily Units × 30`

One electricity unit is equal to one kilowatt-hour (kWh).

### Estimated bill
The program uses these illustrative progressive slabs:

| Consumption slab | Sample rate |
|---|---:|
| First 100 units | Rs. 3 per unit |
| Next 100 units | Rs. 5 per unit |
| Next 300 units | Rs. 7 per unit |
| Above 500 units | Rs. 8 per unit |

**Important:** These rates are examples for demonstrating the program logic. They are not guaranteed to match the user's electricity provider. The estimate excludes fixed charges, taxes, subsidies, and other fees. Update the rates in the source code to match the applicable tariff.

## Technologies and C Concepts
- **Language:** C
- Structures — store appliance details.
- Arrays — manage multiple appliance records.
- Functions — divide the program into logical modules.
- Loops and conditional statements — process data and implement menu choices.
- Sorting — rank appliances by consumption.
- File handling — save and load appliance records.
- Basic arithmetic — calculate energy consumption and estimated charges.

## Requirements
- A C compiler such as GCC, Clang, or an IDE with a C compiler.
- A terminal or command prompt.

No sensors, internet connection, external libraries, or hardware are required. Appliance data is entered manually.

## How to Compile and Run

### Linux / macOS
```bash
gcc smart_electricity.c -o smart_electricity
./smart_electricity
```

### Windows (MinGW GCC)
```bash
gcc smart_electricity.c -o smart_electricity.exe
smart_electricity.exe
```

## Files
- `smart_electricity.c` — main C source code.
- `appliances.dat` — created automatically at runtime to store appliance records.

Keep `appliances.dat` in the same working directory when you want the program to load previously saved records. The data file is created automatically after records are saved.

## How to Use
1. Compile and run the C program.
2. Select **Add Appliance** and enter the appliance details.
3. Add other appliances as required.
4. Use **View Appliances** to verify the entries.
5. Select **Calculate Electricity Consumption** to see daily and monthly units.
6. Select **Estimate Monthly Electricity Bill** to view the sample bill estimate.
7. Explore the ranking, high-consumption detection, and energy-saving suggestions.
8. Select **Generate Complete Report** for a summary.
9. Choose **Save and Exit** to save the records and close the program.

## Limitations
- Consumption is based on user-entered power ratings and average usage hours; it is not measured by a physical electricity meter.
- The program assumes a 30-day month.
- The bill estimate uses illustrative tariff slabs and excludes additional charges.
- High consumption is an indicator for review, not proof of an appliance fault or energy wastage.
- Suggestions are general guidance, not guaranteed savings.

## Possible Future Enhancements
- Allow users to configure tariff rates from the menu.
- Add date-wise and month-wise consumption history.
- Compare current usage with previous records.
- Export reports to CSV.
- Integrate real-time readings from a smart meter or sensor in a future hardware-enabled version.

## Author
LTPS Project — Smart Electricity Consumption Analyzer

---
**Note:** This project is an educational simulation intended to demonstrate C programming and logical problem-solving.
