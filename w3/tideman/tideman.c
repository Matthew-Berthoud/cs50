#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
int find_source(int source);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++) // loop through candidates
    {
        if (strcmp(name, candidates[i]) == 0) // name is a candidate
        {
            ranks[rank] = i; // candidate i is this voter's "rankth" preference
            return true;
        }
    }
    return false; // name didn't belong to any candidate
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++) // loop through or ranks[] indices
    {
        for (int j = i + 1; j < candidate_count; j++) // rest of ranks[] indices
        {
            preferences[ranks[i]][ranks[j]]++;
            // add preference for candidate over every candidate after it in list
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++) // preferences[i]
    {
        for (int j = 0; j < candidate_count; j++) // preferences[i][j]
        {
            if (preferences[i][j] != 0 && preferences[i][j] > preferences[j][i])
            {
                pair cur;
                cur.winner = i;
                cur.loser = j;
                pairs[pair_count] = cur;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int strengths[pair_count]; // array of ints of size pair_count
    for (int i = 0; i < pair_count; i++) // for each pair
    {
        // check strength
        int strength = preferences[pairs[i].winner][pairs[i].loser];
        // record in strengths array to corresponding index
        strengths[i] = strength;
    }

    // selection sort on pairs array, using strengths array
    for (int i = 0; i < pair_count; i++)
    {
        for (int j = i + 1; j < pair_count; j++)
        {
            if (strengths[j] > strengths[i]) // out of order
            { //swap
                pair old = pairs[i];
                pair new = pairs[j];
                pairs[j] = old;
                pairs[i] = new;
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++) // for each subsequent pair
    {
        if (find_source(pairs[i].winner) == pairs[i].loser) // ADDING THIS WOULD CYCLE
        {
            continue;
        }
        locked[pairs[i].winner][pairs[i].loser] = true; // lock the pair
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for(int j = 0; j < candidate_count; j++)
        {
            if (locked[i][j])
            {
                printf("%s\n", candidates[find_source(i)]);
                return;
            }
        }
    }
}

int find_source(int source)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[i][source]) // i is the source of current source
        {
            return find_source(i);
        }
    } // there are no sources for current source, so it is the source
    return source;
}