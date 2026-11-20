#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NAME_SIZE 50
#define PHONE_SIZE 20
#define ADDRESS_SIZE 100
#define MAX_REPAYMENTS 100

/* =========================
   STRUCTURES
   ========================= */

typedef struct {
    int borrowerId;
    char name[NAME_SIZE];
    int age;
    char phone[PHONE_SIZE];
    char address[ADDRESS_SIZE];
    int totalLoans;
    int riskScore;
    int active;
} Borrower;

typedef struct {
    int loanId;
    int borrowerId;

    double principal;
    double annualRate;
    int tenure;

    double emi;
    double totalInterest;
    double totalPayable;
    double outstanding;

    int paidEMIs;
    int remainingEMIs;
    int overdueEMIs;

    double totalPenalty;

    char startDate[15];
    char nextDueDate[15];

    int status;   /* 1 = ACTIVE, 2 = COMPLETED, 3 = OVERDUE */
} Loan;

typedef struct {
    int repaymentId;
    int loanId;
    int borrowerId;

    char date[15];

    double amount;
    int emiNumber;
    int overdueDays;
    double penalty;

    char type[15];
} Repayment;

/* =========================
   GLOBAL DATA
   ========================= */

Borrower *borrowers = NULL;
Loan *loans = NULL;
Repayment *repayments = NULL;

int borrowerCount = 0;
int loanCount = 0;
int repaymentCount = 0;

int borrowerCapacity = 2;
int loanCapacity = 2;
int repaymentCapacity = 10;

/* =========================
   FUNCTION PROTOTYPES
   ========================= */

double calculateEMI(double principal, double annualRate, int tenure);

double calculatePenalty(double overdueAmount,
                        double penaltyRate,
                        int overdueDays);

void addBorrower();
void displayBorrowers();
void searchBorrower();

void addLoan();
void displayLoans();
void searchLoan();

void generateEMISchedule(Loan *loan);
void recordRepayment();

void displayRepaymentHistory();
void calculateRiskScores();
const char *getRiskCategory(int score);

void displayDashboard();
void displayRiskRanking();

void saveData();
void loadData();

void initializeMemory();
void freeMemory();

int findBorrowerIndex(int id);
int findLoanIndex(int id);

void clearInputBuffer();

/* =========================
   MAIN
   ========================= */

int main() {
    int choice;

    initializeMemory();
    loadData();

    do {
        printf("\n====================================================\n");
        printf("       MICROFINANCE LOAN REPAYMENT TRACKER\n");
        printf("====================================================\n");
        printf("1.  Add Borrower\n");
        printf("2.  View Borrowers\n");
        printf("3.  Search Borrower\n");
        printf("4.  Add Loan\n");
        printf("5.  View Loans\n");
        printf("6.  Search Loan\n");
        printf("7.  Generate EMI Schedule\n");
        printf("8.  Record Repayment\n");
        printf("9.  View Repayment History\n");
        printf("10. Update Risk Scores\n");
        printf("11. Portfolio Dashboard\n");
        printf("12. Borrower Risk Ranking\n");
        printf("13. Save Data\n");
        printf("0.  Exit\n");
        printf("====================================================\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1:
                addBorrower();
                break;

            case 2:
                displayBorrowers();
                break;

            case 3:
                searchBorrower();
                break;

            case 4:
                addLoan();
                break;

            case 5:
                displayLoans();
                break;

            case 6:
                searchLoan();
                break;

            case 7: {
                int id;
                int index;

                printf("Enter Loan ID: ");
                scanf("%d", &id);
                clearInputBuffer();

                index = findLoanIndex(id);

                if (index == -1) {
                    printf("Loan not found.\n");
                } else {
                    generateEMISchedule(&loans[index]);
                }
                break;
            }

            case 8:
                recordRepayment();
                break;

            case 9:
                displayRepaymentHistory();
                break;

            case 10:
                calculateRiskScores();
                break;

            case 11:
                displayDashboard();
                break;

            case 12:
                displayRiskRanking();
                break;

            case 13:
                saveData();
                break;

            case 0:
                saveData();
                freeMemory();
                printf("\nThank you for using the system!\n");
                break;

            default:
                printf("Invalid choice. Try again.\n");
        }

    } while (choice != 0);

    return 0;
}

/* =========================
   MEMORY MANAGEMENT
   ========================= */

void initializeMemory() {

    borrowers = malloc(borrowerCapacity * sizeof(Borrower));
    loans = malloc(loanCapacity * sizeof(Loan));
    repayments = malloc(repaymentCapacity * sizeof(Repayment));

    if (borrowers == NULL || loans == NULL || repayments == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
}

void freeMemory() {
    free(borrowers);
    free(loans);
    free(repayments);

    borrowers = NULL;
    loans = NULL;
    repayments = NULL;
}

/* =========================
   INPUT BUFFER
   ========================= */

void clearInputBuffer() {
    int c;

    while ((c = getchar()) != '\n' && c != EOF);
}

/* =========================
   FIND FUNCTIONS
   ========================= */

int findBorrowerIndex(int id) {

    for (int i = 0; i < borrowerCount; i++) {
        if (borrowers[i].borrowerId == id) {
            return i;
        }
    }

    return -1;
}

int findLoanIndex(int id) {

    for (int i = 0; i < loanCount; i++) {
        if (loans[i].loanId == id) {
            return i;
        }
    }

    return -1;
}

/* =========================
   EMI CALCULATION
   ========================= */

double calculateEMI(double principal,
                    double annualRate,
                    int tenure) {

    double monthlyRate;
    double emi;

    monthlyRate = annualRate / (12.0 * 100.0);

    if (monthlyRate == 0) {
        return principal / tenure;
    }

    emi = principal *
          monthlyRate *
          pow(1 + monthlyRate, tenure)
          /
          (pow(1 + monthlyRate, tenure) - 1);

    return emi;
}

/* =========================
   PENALTY CALCULATION
   ========================= */

double calculatePenalty(double overdueAmount,
                        double penaltyRate,
                        int overdueDays) {

    return overdueAmount *
           penaltyRate / 100.0 *
           overdueDays;
}

/* =========================
   ADD BORROWER
   ========================= */

void addBorrower() {

    if (borrowerCount >= borrowerCapacity) {

        borrowerCapacity *= 2;

        borrowers = realloc(
            borrowers,
            borrowerCapacity * sizeof(Borrower)
        );

        if (borrowers == NULL) {
            printf("Memory allocation failed.\n");
            exit(1);
        }
    }

    Borrower *b = &borrowers[borrowerCount];

    printf("\nEnter Borrower ID: ");
    scanf("%d", &b->borrowerId);
    clearInputBuffer();

    if (findBorrowerIndex(b->borrowerId) != -1) {
        printf("Borrower ID already exists.\n");
        return;
    }

    printf("Enter Name: ");
    fgets(b->name, NAME_SIZE, stdin);
    b->name[strcspn(b->name, "\n")] = '\0';

    printf("Enter Age: ");
    scanf("%d", &b->age);
    clearInputBuffer();

    printf("Enter Phone: ");
    fgets(b->phone, PHONE_SIZE, stdin);
    b->phone[strcspn(b->phone, "\n")] = '\0';

    printf("Enter Address: ");
    fgets(b->address, ADDRESS_SIZE, stdin);
    b->address[strcspn(b->address, "\n")] = '\0';

    b->totalLoans = 0;
    b->riskScore = 100;
    b->active = 1;

    borrowerCount++;

    printf("\nBorrower added successfully!\n");
}

/* =========================
   DISPLAY BORROWERS
   ========================= */

void displayBorrowers() {

    if (borrowerCount == 0) {
        printf("\nNo borrowers available.\n");
        return;
    }

    printf("\n====================================================\n");
    printf("                 BORROWER LIST\n");
    printf("====================================================\n");

    for (int i = 0; i < borrowerCount; i++) {

        printf("\nBorrower ID : %d\n",
               borrowers[i].borrowerId);

        printf("Name        : %s\n",
               borrowers[i].name);

        printf("Age         : %d\n",
               borrowers[i].age);

        printf("Phone       : %s\n",
               borrowers[i].phone);

        printf("Address     : %s\n",
               borrowers[i].address);

        printf("Total Loans : %d\n",
               borrowers[i].totalLoans);

        printf("Risk Score  : %d\n",
               borrowers[i].riskScore);

        printf("Risk Level  : %s\n",
               getRiskCategory(borrowers[i].riskScore));

        printf("--------------------------------------------\n");
    }
}

/* =========================
   SEARCH BORROWER
   ========================= */

void searchBorrower() {

    int id;

    printf("Enter Borrower ID: ");
    scanf("%d", &id);
    clearInputBuffer();

    int index = findBorrowerIndex(id);

    if (index == -1) {
        printf("Borrower not found.\n");
        return;
    }

    Borrower *b = &borrowers[index];

    printf("\nBorrower Found\n");
    printf("-----------------------------\n");
    printf("ID          : %d\n", b->borrowerId);
    printf("Name        : %s\n", b->name);
    printf("Age         : %d\n", b->age);
    printf("Phone       : %s\n", b->phone);
    printf("Address     : %s\n", b->address);
    printf("Loans       : %d\n", b->totalLoans);
    printf("Risk Score  : %d\n", b->riskScore);
    printf("Risk Level  : %s\n",
           getRiskCategory(b->riskScore));
}

/* =========================
   ADD LOAN
   ========================= */

void addLoan() {

    if (loanCount >= loanCapacity) {

        loanCapacity *= 2;

        loans = realloc(
            loans,
            loanCapacity * sizeof(Loan)
        );

        if (loans == NULL) {
            printf("Memory allocation failed.\n");
            exit(1);
        }
    }

    Loan *l = &loans[loanCount];

    printf("\nEnter Loan ID: ");
    scanf("%d", &l->loanId);
    clearInputBuffer();

    if (findLoanIndex(l->loanId) != -1) {
        printf("Loan ID already exists.\n");
        return;
    }

    printf("Enter Borrower ID: ");
    scanf("%d", &l->borrowerId);
    clearInputBuffer();

    int borrowerIndex =
        findBorrowerIndex(l->borrowerId);

    if (borrowerIndex == -1) {
        printf("Borrower does not exist.\n");
        return;
    }

    printf("Enter Principal Amount: ");
    scanf("%lf", &l->principal);

    printf("Enter Annual Interest Rate (%%): ");
    scanf("%lf", &l->annualRate);

    printf("Enter Tenure in Months: ");
    scanf("%d", &l->tenure);

    clearInputBuffer();

    if (l->principal <= 0 ||
        l->annualRate < 0 ||
        l->tenure <= 0) {

        printf("Invalid loan details.\n");
        return;
    }

    printf("Enter Start Date (DD-MM-YYYY): ");
    fgets(l->startDate, 15, stdin);
    l->startDate[strcspn(l->startDate, "\n")] = '\0';

    printf("Enter Next Due Date: ");
    fgets(l->nextDueDate, 15, stdin);
    l->nextDueDate[strcspn(l->nextDueDate, "\n")] = '\0';

    l->emi = calculateEMI(
        l->principal,
        l->annualRate,
        l->tenure
    );

    l->totalPayable =
        l->emi * l->tenure;

    l->totalInterest =
        l->totalPayable - l->principal;

    l->outstanding = l->principal;

    l->paidEMIs = 0;
    l->remainingEMIs = l->tenure;
    l->overdueEMIs = 0;
    l->totalPenalty = 0;

    l->status = 1;

    borrowers[borrowerIndex].totalLoans++;

    loanCount++;

    printf("\nLoan created successfully!\n");

    printf("Monthly EMI : %.2f\n", l->emi);
    printf("Total Interest : %.2f\n",
           l->totalInterest);
    printf("Total Payable : %.2f\n",
           l->totalPayable);
}

/* =========================
   DISPLAY LOANS
   ========================= */

void displayLoans() {

    if (loanCount == 0) {
        printf("\nNo loans available.\n");
        return;
    }

    printf("\n====================================================\n");
    printf("                    LOAN LIST\n");
    printf("====================================================\n");

    for (int i = 0; i < loanCount; i++) {

        Loan *l = &loans[i];

        printf("\nLoan ID          : %d\n", l->loanId);
        printf("Borrower ID      : %d\n", l->borrowerId);
        printf("Principal        : %.2f\n", l->principal);
        printf("Interest Rate    : %.2f%%\n",
               l->annualRate);
        printf("Tenure           : %d months\n",
               l->tenure);
        printf("Monthly EMI      : %.2f\n", l->emi);
        printf("Outstanding      : %.2f\n",
               l->outstanding);
        printf("Paid EMIs        : %d\n",
               l->paidEMIs);
        printf("Remaining EMIs   : %d\n",
               l->remainingEMIs);
        printf("Overdue EMIs     : %d\n",
               l->overdueEMIs);
        printf("Total Penalty    : %.2f\n",
               l->totalPenalty);

        if (l->status == 1)
            printf("Status           : ACTIVE\n");
        else if (l->status == 2)
            printf("Status           : COMPLETED\n");
        else
            printf("Status           : OVERDUE\n");

        printf("--------------------------------------------\n");
    }
}

/* =========================
   SEARCH LOAN
   ========================= */

void searchLoan() {

    int id;

    printf("Enter Loan ID: ");
    scanf("%d", &id);
    clearInputBuffer();

    int index = findLoanIndex(id);

    if (index == -1) {
        printf("Loan not found.\n");
        return;
    }

    Loan *l = &loans[index];

    printf("\nLoan Details\n");
    printf("--------------------------------\n");
    printf("Loan ID        : %d\n", l->loanId);
    printf("Borrower ID    : %d\n", l->borrowerId);
    printf("Principal      : %.2f\n", l->principal);
    printf("Interest Rate  : %.2f%%\n",
           l->annualRate);
    printf("Tenure         : %d months\n",
           l->tenure);
    printf("EMI            : %.2f\n", l->emi);
    printf("Outstanding    : %.2f\n",
           l->outstanding);
    printf("Paid EMIs      : %d\n",
           l->paidEMIs);
    printf("Remaining EMIs : %d\n",
           l->remainingEMIs);
}

/* =========================
   EMI SCHEDULE
   ========================= */

void generateEMISchedule(Loan *loan) {

    double balance = loan->principal;
    double monthlyRate =
        loan->annualRate / (12.0 * 100.0);

    printf("\n===============================================================\n");
    printf("                     EMI AMORTIZATION SCHEDULE\n");
    printf("===============================================================\n");

    printf("Loan ID : %d\n", loan->loanId);
    printf("Monthly EMI : %.2f\n\n", loan->emi);

    printf("---------------------------------------------------------------\n");
    printf("No. | Opening | EMI | Interest | Principal | Closing\n");
    printf("---------------------------------------------------------------\n");

    for (int i = 1; i <= loan->tenure; i++) {

        double interest =
            balance * monthlyRate;

        double principalPart =
            loan->emi - interest;

        double closing =
            balance - principalPart;

        if (i == loan->tenure) {
            principalPart = balance;
            loan->emi =
                principalPart + interest;
            closing = 0;
        }

        printf("%3d | %8.2f | %8.2f | %8.2f | %9.2f | %8.2f\n",
               i,
               balance,
               loan->emi,
               interest,
               principalPart,
               closing);

        balance = closing;

        if (balance <= 0)
            break;
    }

    printf("---------------------------------------------------------------\n");
}

/* =========================
   RECORD REPAYMENT
   ========================= */

void recordRepayment() {

    int loanId;
    double amount;
    int overdueDays;

    printf("\nEnter Loan ID: ");
    scanf("%d", &loanId);

    int loanIndex = findLoanIndex(loanId);

    if (loanIndex == -1) {
        printf("Loan not found.\n");
        clearInputBuffer();
        return;
    }

    Loan *loan = &loans[loanIndex];

    if (loan->status == 2) {
        printf("This loan is already completed.\n");
        clearInputBuffer();
        return;
    }

    printf("Enter repayment amount: ");
    scanf("%lf", &amount);

    if (amount <= 0 ||
        amount > loan->outstanding) {

        printf("Invalid repayment amount.\n");
        clearInputBuffer();
        return;
    }

    printf("Enter overdue days (0 if on time): ");
    scanf("%d", &overdueDays);

    clearInputBuffer();

    if (overdueDays < 0) {
        overdueDays = 0;
    }

    /* Expand repayment memory */

    if (repaymentCount >= repaymentCapacity) {

        repaymentCapacity *= 2;

        repayments = realloc(
            repayments,
            repaymentCapacity *
            sizeof(Repayment)
        );

        if (repayments == NULL) {
            printf("Memory allocation failed.\n");
            exit(1);
        }
    }

    Repayment *r =
        &repayments[repaymentCount];

    r->repaymentId =
        repaymentCount + 1;

    r->loanId = loan->loanId;
    r->borrowerId = loan->borrowerId;

    printf("Enter payment date: ");
    fgets(r->date, 15, stdin);
    r->date[strcspn(r->date, "\n")] = '\0';

    r->amount = amount;

    r->emiNumber =
        loan->paidEMIs + 1;

    r->overdueDays = overdueDays;

    if (overdueDays > 0) {

        r->penalty =
            calculatePenalty(
                amount,
                2.0,
                overdueDays
            );

        strcpy(r->type, "LATE");

        loan->totalPenalty +=
            r->penalty;

        loan->overdueEMIs++;

        loan->status = 3;

    } else {

        r->penalty = 0;

        strcpy(r->type, "ON TIME");
    }

    /* Update outstanding */

    loan->outstanding -= amount;

    if (amount >= loan->emi) {
        loan->paidEMIs++;
    }

    loan->remainingEMIs =
        loan->tenure - loan->paidEMIs;

    if (loan->outstanding <= 0.01) {

        loan->outstanding = 0;

        loan->remainingEMIs = 0;

        loan->status = 2;

        printf("\nLoan completed!\n");

    } else {

        if (overdueDays == 0)
            loan->status = 1;

        printf("\nRepayment recorded successfully!\n");
    }

    repaymentCount++;

    printf("Outstanding Balance : %.2f\n",
           loan->outstanding);

    printf("Penalty             : %.2f\n",
           r->penalty);
}

/* =========================
   REPAYMENT HISTORY
   ========================= */

void displayRepaymentHistory() {

    int borrowerId;

    printf("\nEnter Borrower ID: ");
    scanf("%d", &borrowerId);
    clearInputBuffer();

    int found = 0;

    printf("\n====================================================\n");
    printf("              REPAYMENT HISTORY\n");
    printf("====================================================\n");

    for (int i = 0; i < repaymentCount; i++) {

        if (repayments[i].borrowerId ==
            borrowerId) {

            found = 1;

            printf("\nRepayment ID : %d\n",
                   repayments[i].repaymentId);

            printf("Loan ID      : %d\n",
                   repayments[i].loanId);

            printf("Date         : %s\n",
                   repayments[i].date);

            printf("EMI Number   : %d\n",
                   repayments[i].emiNumber);

            printf("Amount       : %.2f\n",
                   repayments[i].amount);

            printf("Type         : %s\n",
                   repayments[i].type);

            printf("Overdue Days : %d\n",
                   repayments[i].overdueDays);

            printf("Penalty      : %.2f\n",
                   repayments[i].penalty);

            printf("--------------------------------------------\n");
        }
    }

    if (!found) {
        printf("No repayment records found.\n");
    }
}

/* =========================
   RISK CATEGORY
   ========================= */

const char *getRiskCategory(int score) {

    if (score >= 75)
        return "LOW RISK";

    if (score >= 50)
        return "MEDIUM RISK";

    return "HIGH RISK";
}

/* =========================
   RISK SCORE
   ========================= */

void calculateRiskScores() {

    for (int i = 0;
         i < borrowerCount;
         i++) {

        int score = 100;

        for (int j = 0;
             j < repaymentCount;
             j++) {

            if (repayments[j].borrowerId ==
                borrowers[i].borrowerId) {

                if (repayments[j].overdueDays > 0)
                    score -= 10;

                if (strcmp(
                    repayments[j].type,
                    "LATE") == 0)
                    score -= 5;
            }
        }

        if (score < 0)
            score = 0;

        borrowers[i].riskScore = score;
    }

    printf("\nRisk scores updated successfully.\n");
}

/* =========================
   PORTFOLIO DASHBOARD
   ========================= */

void displayDashboard() {

    double totalPrincipal = 0;
    double totalOutstanding = 0;
    double totalInterest = 0;
    double totalPenalty = 0;

    int activeLoans = 0;
    int completedLoans = 0;
    int overdueLoans = 0;

    for (int i = 0; i < loanCount; i++) {

        totalPrincipal +=
            loans[i].principal;

        totalOutstanding +=
            loans[i].outstanding;

        totalInterest +=
            loans[i].totalInterest;

        totalPenalty +=
            loans[i].totalPenalty;

        if (loans[i].status == 1)
            activeLoans++;

        else if (loans[i].status == 2)
            completedLoans++;

        else if (loans[i].status == 3)
            overdueLoans++;
    }

    double delinquencyRate = 0;

    if (activeLoans > 0) {

        delinquencyRate =
            ((double)overdueLoans /
             activeLoans) * 100;
    }

    int low = 0;
    int medium = 0;
    int high = 0;

    for (int i = 0;
         i < borrowerCount;
         i++) {

        if (borrowers[i].riskScore >= 75)
            low++;

        else if (borrowers[i].riskScore >= 50)
            medium++;

        else
            high++;
    }

    printf("\n====================================================\n");
    printf("             MICROFINANCE DASHBOARD\n");
    printf("====================================================\n");

    printf("Total Borrowers       : %d\n",
           borrowerCount);

    printf("Active Loans          : %d\n",
           activeLoans);

    printf("Completed Loans       : %d\n",
           completedLoans);

    printf("Overdue Loans         : %d\n",
           overdueLoans);

    printf("--------------------------------------------\n");

    printf("Total Principal       : %.2f\n",
           totalPrincipal);

    printf("Total Outstanding     : %.2f\n",
           totalOutstanding);

    printf("Total Interest        : %.2f\n",
           totalInterest);

    printf("Total Penalties       : %.2f\n",
           totalPenalty);

    printf("Delinquency Rate      : %.2f%%\n",
           delinquencyRate);

    printf("--------------------------------------------\n");

    printf("Low Risk Borrowers    : %d\n", low);
    printf("Medium Risk Borrowers : %d\n", medium);
    printf("High Risk Borrowers   : %d\n", high);

    printf("====================================================\n");
}

/* =========================
   RISK RANKING
   ========================= */

void displayRiskRanking() {

    if (borrowerCount == 0) {
        printf("No borrowers available.\n");
        return;
    }

    /* Create temporary array */

    Borrower *temp =
        malloc(borrowerCount *
               sizeof(Borrower));

    if (temp == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    for (int i = 0;
         i < borrowerCount;
         i++) {

        temp[i] = borrowers[i];
    }

    /* Sort highest risk first */

    for (int i = 0;
         i < borrowerCount - 1;
         i++) {

        for (int j = 0;
             j < borrowerCount - i - 1;
             j++) {

            if (temp[j].riskScore >
                temp[j + 1].riskScore) {

                Borrower t = temp[j];

                temp[j] =
                    temp[j + 1];

                temp[j + 1] = t;
            }
        }
    }

    printf("\n====================================================\n");
    printf("             BORROWER RISK RANKING\n");
    printf("====================================================\n");

    printf("Rank | ID | Name | Score | Category\n");
    printf("----------------------------------------------------\n");

    for (int i = 0;
         i < borrowerCount;
         i++) {

        printf("%4d | %d | %s | %d | %s\n",
               i + 1,
               temp[i].borrowerId,
               temp[i].name,
               temp[i].riskScore,
               getRiskCategory(
                   temp[i].riskScore));
    }

    free(temp);
}

/* =========================
   FILE HANDLING
   ========================= */

void saveData() {

    FILE *fp;

    fp = fopen("borrowers.dat", "wb");

    if (fp != NULL) {

        fwrite(&borrowerCount,
               sizeof(int),
               1,
               fp);

        fwrite(borrowers,
               sizeof(Borrower),
               borrowerCount,
               fp);

        fclose(fp);
    }

    fp = fopen("loans.dat", "wb");

    if (fp != NULL) {

        fwrite(&loanCount,
               sizeof(int),
               1,
               fp);

        fwrite(loans,
               sizeof(Loan),
               loanCount,
               fp);

        fclose(fp);
    }

    fp = fopen("repayments.dat", "wb");

    if (fp != NULL) {

        fwrite(&repaymentCount,
               sizeof(int),
               1,
               fp);

        fwrite(repayments,
               sizeof(Repayment),
               repaymentCount,
               fp);

        fclose(fp);
    }

    printf("\nData saved successfully.\n");
}

/* =========================
   LOAD DATA
   ========================= */

void loadData() {

    FILE *fp;

    fp = fopen("borrowers.dat", "rb");

    if (fp != NULL) {

        fread(&borrowerCount,
              sizeof(int),
              1,
              fp);

        while (borrowerCount >= borrowerCapacity) {

            borrowerCapacity *= 2;

            borrowers =
                realloc(
                    borrowers,
                    borrowerCapacity *
                    sizeof(Borrower)
                );
        }

        fread(borrowers,
              sizeof(Borrower),
              borrowerCount,
              fp);

        fclose(fp);
    }

    fp = fopen("loans.dat", "rb");

    if (fp != NULL) {

        fread(&loanCount,
              sizeof(int),
              1,
              fp);

        while (loanCount >= loanCapacity) {

            loanCapacity *= 2;

            loans =
                realloc(
                    loans,
                    loanCapacity *
                    sizeof(Loan)
                );
        }

        fread(loans,
              sizeof(Loan),
              loanCount,
              fp);

        fclose(fp);
    }

    fp = fopen("repayments.dat", "rb");

    if (fp != NULL) {

        fread(&repaymentCount,
              sizeof(int),
              1,
              fp);

        while (repaymentCount >=
               repaymentCapacity) {

            repaymentCapacity *= 2;

            repayments =
                realloc(
                    repayments,
                    repaymentCapacity *
                    sizeof(Repayment)
                );
        }

        fread(repayments,
              sizeof(Repayment),
              repaymentCount,
              fp);

        fclose(fp);
    }
}
