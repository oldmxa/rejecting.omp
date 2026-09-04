#if defined _clan_config_included
    #endinput
#endif
#define _clan_config_included

#define MAX_CLANS               (100)
#define MAX_MEMBERS             (31)
#define MAX_ROLES               (5)

#define CLAN_BASE_SLOTS         (5)

#define MAX_CLAN_NAME           (20)
#define MAX_CLAN_TAG            (6)
#define MAX_ROLE_NAME           (20)

#define CLAN_NAME_LEN           (MAX_CLAN_NAME + 1)
#define CLAN_TAG_LEN            (MAX_CLAN_TAG + 1)
#define CLAN_ROLE_LEN           (MAX_ROLE_NAME + 1)

#define CLAN_NAME_MIN           (2)
#define CLAN_TAG_MIN            (3)
#define CLAN_ROLE_MIN           (2)

#define CLAN_TEXT_FREE          (0)
#define CLAN_TEXT_TAG_TAKEN     (1)
#define CLAN_TEXT_NAME_TAKEN    (2)

#define CLAN_PRICE              (500000)
#define CLAN_COLOR_PRICE        (3000000)

// alpha applied to the clan color so the tab list stays readable
#define CLAN_COLOR_ALPHA        (0xAA)
#define CLAN_COLOR_ALPHA_CMD    (0x59)
#define CLAN_COLOR_DEFAULT      (0xa4aba865)

// an invite is only redeemable by /accept inside this window
#define CLAN_INVITE_TIMEOUT     (20)

#define CLAN_MEMBERS_PER_PAGE   (10)
#define CLAN_TERRITORY_PER_PAGE (20)
#define CLAN_LOG_ROWS           (20)

// points the clan that dealt most damage takes from a finished round
#define CLAN_ROUND_POINTS       (50)

// hourly payout per owned zone, by zone category
#define CLAN_REWARD_BIG         (5000)
#define CLAN_REWARD_MEDIUM      (2500)
#define CLAN_REWARD_SMALL       (1500)

enum
{
    RANK_TRIAL,
    RANK_MEMBER,
    RANK_RECRUTER,
    RANK_COFOUNDER,
    RANK_FOUNDER
};
