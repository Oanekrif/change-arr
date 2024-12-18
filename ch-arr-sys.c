#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ADMINS 10
#define MAX_NAME_LEN 20
#define MAX_TRIES 5
#define DATA_FILE "data.txt"

struct user_data
{
    char name[MAX_NAME_LEN];
    int psw;
};

void print_array(int arr[], int size)
{
    printf("The array is = {");
    for (int i = 0; i < size; i++)
    {
        printf("%d", arr[i]);
        if (i < size - 1)
        {
            printf(", ");
        }
    }
    printf("}\n");
}

int is_admin(struct user_data users[], int numAdmins, char *name)
{
    for (int i = 0; i < numAdmins; i++)
    {
        if (strcmp(name, users[i].name) == 0)
        {
            return i;
        }
    }
    return -1;
}

void update_array(int arr[], int size)
{
    printf("Enter the new array: ");
    for (int i = 0; i < size; i++)
    {
        scanf("%d", &arr[i]);
    }
    print_array(arr, size);
}

void add_admin(struct user_data users[], int *numAdmins)
{
    if (*numAdmins < MAX_ADMINS)
    {
        printf("Please enter new admin name: ");
        scanf("%s", users[*numAdmins].name);
        printf("Please enter new admin password: ");
        scanf("%d", &users[*numAdmins].psw);
        (*numAdmins)++;
        printf("Congratulations! You are now an admin. Your admin status will be active from the next session.\n");
    }
    else
    {
        printf("Cannot add more admins. Maximum limit reached.\n");
    }
    printf("============================================\n");
}

void save_data(struct user_data users[], int numAdmins, int arr[], int size)
{
    FILE *file = fopen(DATA_FILE, "wb");
    if (file == NULL)
    {
        printf("Error opening file for writing.\n");
        return;
    }
    fwrite(&numAdmins, sizeof(int), 1, file);
    fwrite(users, sizeof(struct user_data), numAdmins, file);
    fwrite(arr, sizeof(int), size, file);
    fclose(file);
}

void load_data(struct user_data users[], int *numAdmins, int arr[], int size)
{
    FILE *file = fopen(DATA_FILE, "rb");
    if (file == NULL)
    {
        printf("No previous data found, starting fresh.\n");
        *numAdmins = 0;
        for (int i = 0; i < size; i++)
        {
            arr[i] = i + 1;
        }
        return;
    }
    fread(numAdmins, sizeof(int), 1, file);
    fread(users, sizeof(struct user_data), *numAdmins, file);
    fread(arr, sizeof(int), size, file);
    fclose(file);
}

int main()
{
    struct user_data users[MAX_ADMINS];
    int numAdmins;
    int arr[5];

    load_data(users, &numAdmins, arr, 5);
    if (numAdmins == 0)
    {
        strcpy(users[0].name, "omar");
        users[0].psw = 1234;
        numAdmins = 1;
    }

    printf("============================================\n");
    printf("Hello user to this program\n");
    print_array(arr, 5);

    while (1)
    {
        printf("============================================\n");
        printf("Please enter your user name (or type 'exit' or 'q' to quit): ");
        char choys[MAX_NAME_LEN];
        scanf("%s", choys);

        if (strcmp(choys, "exit") == 0 || strcmp(choys, "q") == 0)
        {
            printf("Program finished.\n");
            save_data(users, numAdmins, arr, 5);
            break;
        }

        int adminIndex = is_admin(users, numAdmins, choys);

        if (adminIndex != -1)
        {
            printf("Please enter your password: ");
            int enteredPsw;
            scanf("%d", &enteredPsw);

            int attempts = MAX_TRIES;
            while (attempts > 0)
            {
                if (enteredPsw == users[adminIndex].psw)
                {
                    printf("============================================\n");
                    printf("Hello admin: %s\n", users[adminIndex].name);
                    print_array(arr, 5);
                    printf("If you want to change the array, enter 'yes' or 'y': ");
                    char check[5];
                    scanf("%s", check);
                    if (strcmp(check, "yes") == 0 || strcmp(check, "y") == 0)
                    {
                        update_array(arr, 5);
                    }
                    save_data(users, numAdmins, arr, 5);
                    break;
                }
                else
                {
                    attempts--;
                    if (attempts > 0)
                    {
                        printf("============================================\n");
                        printf("The password is incorrect, %d tries left\n", attempts);
                        printf("Please enter your password: ");
                        scanf("%d", &enteredPsw);
                    }
                    else
                    {
                        printf("============================================\n");
                        printf("The password is incorrect, no attempts left\n");
                    }
                }
            }
        }
        else
        {
            printf("============================================\n");
            printf("You are not an admin\n");
            printf("If you want to become an admin, enter 'yes' or 'y': ");
            char check[5];
            scanf("%s", check);
            if (strcmp(check, "yes") == 0 || strcmp(check, "y") == 0)
            {
                add_admin(users, &numAdmins);
                save_data(users, numAdmins, arr, 5);
            }
        }
    }
    return 0;
}
