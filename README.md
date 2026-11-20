# 💰 Microfinance Loan Repayment Tracker & EMI Planner

## 📌 Project Overview

The **Microfinance Loan Repayment Tracker & EMI Planner** is a menu-driven application developed using the C programming language. It is designed to help microfinance institutions manage borrower information, loan records, EMI calculations, repayment tracking, overdue payments, penalties, and borrower risk analysis.

The system simplifies loan management by calculating monthly EMIs, maintaining repayment records, tracking outstanding balances, and generating portfolio summaries.

This project demonstrates important C programming concepts, including structures, functions, arrays, pointers, dynamic memory allocation, loops, decision-making statements, and file handling.

---

## 🎯 Objectives

- Maintain borrower information and loan records.
- Calculate monthly EMI using the amortization formula.
- Generate loan repayment schedules.
- Record full and partial repayments.
- Calculate penalties for overdue payments.
- Track outstanding loan balances.
- Calculate borrower risk scores.
- Generate portfolio dashboards and risk rankings.
- Store information permanently using file handling.

---

## ✨ Features

### 1. Borrower Management
- Add new borrowers.
- View all registered borrowers.
- Search borrowers using Borrower ID.
- Store borrower details such as name, age, phone number, and address.

### 2. Loan Management
- Create new loan records.
- Store principal amount, interest rate, and loan tenure.
- Calculate monthly EMI automatically.
- Track outstanding loan balances.
- Display loan information and status.

### 3. EMI Calculator
Uses the standard reducing-balance EMI formula:

**EMI = P × r × (1+r)^n / ((1+r)^n − 1)**

Where:

- P = Principal loan amount
- r = Monthly interest rate
- n = Loan tenure in months

### 4. EMI Schedule Generation
Generates an amortization schedule containing:

- EMI number
- Opening balance
- EMI amount
- Interest component
- Principal component
- Closing balance

### 5. Repayment Management
- Record loan repayments.
- Support partial and full repayments.
- Update outstanding balances.
- Maintain repayment history.
- Track overdue days.

### 6. Penalty Calculation

Calculates penalties for overdue repayments using:

**Penalty = Overdue Amount × (Penalty Rate / 100) × Overdue Days**

### 7. Borrower Risk Analysis

Borrowers are classified into three categories:

| Risk Score | Category |
|---|---|
| 75–100 | Low Risk |
| 50–74 | Medium Risk |
| Below 50 | High Risk |

Risk scores are calculated using repayment history.

### 8. Portfolio Dashboard

Displays:

- Total borrowers
- Active loans
- Completed loans
- Overdue loans
- Total principal
- Total outstanding balance
- Total interest
- Total penalties
- Delinquency rate
- Borrower risk distribution

### 9. File Handling

The application stores information in three files:

- `borrowers.dat`
- `loans.dat`
- `repayments.dat`

Existing records can be loaded when the program starts.

### 10. Dynamic Memory Allocation

Uses dynamic memory allocation to expand the portfolio when additional borrowers, loans, or repayment records are added.

Functions used:

- `malloc()`
- `realloc()`
- `free()`

---

## 🛠️ Technologies Used

| Technology | Purpose |
|---|---|
| C Programming | Application development |
| GCC Compiler | Compilation |
| VS Code | Code editing |
| Standard C Library | Input/output and memory management |
| File Handling | Data persistence |

---

## 📚 C Programming Concepts Applied

| Concept | Application |
|---|---|
| Variables and Data Types | Store loan and borrower information |
| Arithmetic Operations | EMI and penalty calculations |
| Decision Making | Loan status and menu operations |
| Loops | Process multiple borrowers and loans |
| Arrays | Manage multiple records |
| Functions | Modular programming |
| Structures | Represent borrowers, loans, and repayments |
| Pointers | Access and modify records |
| Dynamic Memory | Expand the loan portfolio |
| File Handling | Save and retrieve records |
| Sorting | Rank borrowers by risk score |

---

## 📂 Project Structure

```text
Micro
