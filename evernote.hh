#include <vector>
#include <map>
#include <set>


namespace evernote {

	class Timestamp;
	class PremiumOrderStatus;
	class Resource;
	class NoteAttributes;
	class UserID;
	class Publishing;
	class SharedNotebook;
	class User;
	class NotebookRestrictions;
	class ResourceAttributes;
	class SavedSearchScope;
	class SharedNotebookRecipientSettings;
	class UserAttributes;
	class SyncChunkFilter;
	class AuthenticateResult;
	class RelatedResultSpec;
	class PublicUserInfo;
	class BootstrapSettings;

	enum BusinessUserRole { ADMIN = 1, NORMAL = 2 };

	enum SharedNotebookPrivilegeLevel {READ_NOTEBOOK = 0,
		MODIFY_NOTEBOOK_PLUS_ACTIVITY = 1,
		READ_NOTEBOOK_PLUS_ACTIVITY = 2,
		GROUP = 3,
		FULL_ACCESS = 4,
		BUSINESS_FULL_ACCESS = 5,
	};

	enum SharedNotebookInstanceRestrictions {
		ONLY_JOINED_OR_PREVIEW = 1,
		NO_SHARED_NOTEBOOKS = 2,
	};

	enum SponsoredGroupRole {
		GROUP_MEMBER = 1,
		GROUP_ADMIN = 2,
		GROUP_OWNER = 3,
	};

	/** BASIC TYPES */
	class Accounting {
	public:
		long uploadLimit;
		Timestamp* uploadLimitEnd;
		long uploadLimitNextMonth;
		PremiumOrderStatus* premiumServiceStatus;
		std::string premiumOrderNumber;
		std::string premiumCommerceService;
		Timestamp* premiumServiceStart;
		std::string premiumServiceSKU;
		Timestamp* lastSuccessfulCharge;
		Timestamp* lastFailedCharge;
		std::string lastFailedChargeReason;
		Timestamp* nextPaymentDue;
		Timestamp* premiumLockUntil;
		Timestamp* updated;
		std::string premiumSubscriptionNumber;
		Timestamp* lastRequestedCharge;
		std::string currency;
		int unitPrice;
		int businessId;
		std::string businessName;
		BusinessUserRole businessRole;
		int unitDiscount;
		Timestamp* nextChargeDate;
	};

	class BusinessNotebook {
	public:
		std::string notebookDescription;
		SharedNotebookPrivilegeLevel privilege;
		bool recommended;
	};

	class BusinessUserInfo {
	public:
		int businessId;
		std::string businessName;
		BusinessUserRole role;
		std::string email;
	};

	class Data {
	public:
		std::string bodyHash;
		int size;
		std::string body;
	};

	class GUID {
	public:
		std::string guid;
	};

	class LazyMap {
	public:
		std::set<std::string>keysOnly;
		std::map<std::string, std::string> fullMap;
	};

	class LinkedNotebook {
	public:
		std::string shareName;
		std::string username;
		std::string shardId;
		std::string shareKey;
		std::string uri;
		GUID guid;
		int updateSequenceNum;
		std::string noteStoreUrl;
		std::string webApiUrlPrefix;
		std::string stack;
		int businessId;
	};

	class Note {
	public:
		GUID guid;
		std::string title;
		std::string content;
		std::string contentHash;
		int contentLength;
		Timestamp* created;
		Timestamp* updated;
		Timestamp* deleted;
		bool active;
		int updateSequenceNum;
		std::string notebookGuid;
		std::vector<GUID> tagGuids;
		std::vector<Resource*> resources;
		NoteAttributes* attributes;
		std::vector<std::string> tagNames;
	};

	class NoteAttributes {
	public:
		Timestamp* subjectDate;
		double latitude;
		double longitude;
		double altitude;
		std::string author;
		std::string source;
		std::string sourceURL;
		std::string sourceApplication;
		Timestamp* shareDate;
		long reminderOrder;
		Timestamp* reminderDoneTime;
		Timestamp* reminderTime;
		std::string placeName;
		std::string contentClass;
		LazyMap* applicationData;
		std::string lastEditedBy;
		std::map<std::string, std::string> classifications;
		UserID* creatorId;
		UserID* lastEditorId;
	};

	class NoteSortOrder {};

	class Notebook {
	public:
		GUID guid;
		std::string name;
		int updateSequenceNum;
		bool defaultNotebook;
		Timestamp* serviceCreated;
		Timestamp* serviceUpdated;
		Publishing* publishing;
		bool published;
		std::string stack;
		std::vector<long> sharedNotebookIds;
		std::vector<SharedNotebook*> sharedNotebooks;
		BusinessNotebook* businessNotebook;
		User* contact;
		NotebookRestrictions* restrictions;
	};

	class NotebookDescriptor {
	public:
		GUID guid;
		std::string notebookDisplayName;
		std::string contactName;
		bool hasShareNotebook;
		int joinedUserCount;
	};

	class NotebookRestrictions {
	public:
		bool noReadNotes;
		bool noCreateNotes;
		bool noUpdateNotes;
		bool noExpungeNotes;
		bool noShareNotes;
		bool noEmailNotes;
		bool noSendMessageToRecipients;
		bool noUpdateNotebook;
		bool noExpunegNotebook;
		bool noSetdefaultNotebook;
		bool noSetNotebookStack;
		bool noPublishToPublic;
		bool noPublishToBusinessLibrary;
		bool noCreateTags;
		bool noUpdateTags;
		bool noExpungeTags;
		bool noSetParentTag;
		bool noCreateSharedNotebooks;
		SharedNotebookInstanceRestrictions* updateWhichSharedNotebookRestrictions;
		SharedNotebookInstanceRestrictions* expungeWhichSharedNotebookRestrictions;
	};
	
	class PremiumInfo {
	public:
		Timestamp* currentTime;
		bool premium;
		bool premiumRecurring;
		Timestamp* premiumExpirationDate;
		bool premiumExtendable;
		bool premiumPending;
		bool premiumCancellationPending;
		bool canPurchaseUploadAllowance;
		std::string sponsoredGroupName;
		SponsoredGroupRole* sponsoredGroupRole;
		bool premiumUpgradable;
	};

	class PremiumOrderStatus {};

	class PrivilegeLevel {};

	class Publishing {
	public:
		std::string uri;
		NoteSortOrder* order;
		bool ascending;
		std::string publicDescription;
	};

	class QueryFormat {};

	class ReminderEmailConfig {};

	class Resource {
	public:
		GUID guid;
		GUID noteGuid;
		Data* data;
		std::string mime;
		short width;
		short height;
		short duration;
		bool active;
		Data* recognition;
		ResourceAttributes* attributes;
		int updateSequenceNum;
		Data* alternateData;
	};

	class ResourceAttributes {
	public:
		std::string sourceURL;
		Timestamp* timestamp;
		double latitude;
		double longitude;
		double altitude;
		std::string cameraMake;
		std::string cameraModel;
		bool clientWillIndex;
		std::string recoType;
		std::string fileName;
		bool attachment;
		LazyMap* applicationData;
	};

	class SavedSearch {
	public:
		GUID guid;
		std::string name;
		std::string query;
		QueryFormat format;
		int updateSequenceNum;
		SavedSearchScope* scope;
	};

	class SavedSearchScope {
	public:
		bool includeAccount;
		bool includePersonalLinkedNotebooks;
		bool includeBusinessLinkedNotebooks;
	};

	class SharedNotebook {
		long id;
		int userId;
		std::string notebookGuid;
		std::string email;
		bool notebookModifiable;
		bool requireLogin;
		Timestamp* serviceCreated;
		Timestamp* serviceUpdated;
		std::string shareKey;
		std::string username;
		SharedNotebookPrivilegeLevel privilege;
		bool allowPreview;
		SharedNotebookRecipientSettings* recipientSettings;
	};

	class SharedNotebookRecipientSettings {
	public:
		bool reminderNotifyEmail;
		bool reminderNotifyInApp;
	};

	class Tag {
	public:
		GUID guid;
		std::string name;
		GUID parentGuid;
		int updateSequenceNum;
	};

	class Timestamp {
	public:
		long timestamp;
	};

	class User {
	public:
		UserID* id;
		std::string username;
		std::string email;
		std::string name;
		std::string timezone;
		PrivilegeLevel privilege;
		Timestamp* created;
		Timestamp* updated;
		Timestamp* deleted;
		bool active;
		std::string shardId;
		UserAttributes* attributes;
		Accounting* accounting;
		PremiumInfo* premiumInfo;
		BusinessUserInfo* businessUserInfo;
	};

	class UserAttributes {
	public:
		std::string defaultLocationName;
		double defaultLatitude;
		double defaultLongitude;
		bool preactivation;
		std::vector<std::string> viewedPromotions;
		std::string incmoingEmailAddress;
		std::vector<std::string> recentMailedAddresses;
		std::string comments;
		Timestamp* dateAgreedToTermsOfService;
		int maxReferrals;
		int referralCount;
		std::string refererCode;
		Timestamp* sentEmailDate;
		int sentEmailCount;
		int dailyEmailLimit;
		Timestamp* emailOptOutDate;
		Timestamp* partnerEmailOptInDate;
		std::string preferredLanguage;
		std::string preferredCountry;
		bool clipFullPage;
		std::string twitterUserName;
		std::string twitterId;
		std::string groupName;
		std::string recognitionLanguage;
		std::string referralProof;
		bool educationalDiscount;
		std::string businessAddress;
		bool hideSponsorBilling;
		bool taxExempt;
		bool useEmailAutoFiling;
		ReminderEmailConfig* reminderEmailConfig;
	};

	class UserID {
	public:
		int userId;
	};

	/** NOTESTORE TYPES */
	class ClientUsageMetrics {};

	class NoteCollectionCounts {};

	class NoteEmailParameters {};

	class NoteFilter {};

	class NoteList {};

	class NoteMetadata {};

	class NoteVersionId {};

	class NotesMetadataList {};

	class NotesMetadataResultSpec {};

	class RelatedQuery {};

	class RelatedResult {};

	class RelatedResutlSpec {};

	class SyncChunk {};

	class SyncChunlFilter {};

	class SyncState {};

	class NoteStore {
	private:
		evernote::edam::NoteStoreClient* noteStore;
		boost::shared_ptr<apache::thrift::transport::THttpClient> userStoreHttpClient;
	public:
		NoteStore (std::string noteStoreUrl);
		~NoteStore ();
		SyncState* getSyncState (std::string authenticationToken);
		SyncState* getSyncStateWithMetrics (std::string authenticationToken, 
			ClientUsageMetrics* clientMetrics);
		SyncChunk* getFilteredSyncChunk (std::string authenticationToken, 
			int afterUSN,  int maxEntries, SyncChunkFilter* filter);
		SyncState* getLinkedNotebookSyncState (std::string authenticationToken,
			LinkedNotebook* linkedNotebook);
		SyncChunk* getLinkedNotebookSyncChunk (std::string authenticationToken,
			LinkedNotebook* linkedNotebook, int afterUSN, int maxEntries,
			bool fullSyncOnly);
		std::vector<Notebook*>* listNotebooks(std::string authenticationToken);
		Notebook* getNotebook (std::string authenticationToken, GUID guid);
		Notebook* getDefaultNotebook (std::string authenticationToken);
		Notebook* createNotebook (std::string authenticationToken,
			Notebook* notebook);
		int updateNotebook (std::string authenticationToken, Notebook* notebook);
		int expungeNotebook (std::string authenticationToken, GUID guid);
		std::vector<Tag*>* listTags (std::string authenticationToken);
		std::vector<Tag*>* listTagsByNotebook (std::string authenticationToken,
			GUID notebookGuid);
		Tag* getTag (std::string authenticationToken, GUID guid);
		Tag* createTag (std::string authenticationToken, Tag* tag);
		int updateTag (std::string authenticationToken, Tag* tag);
		void untagAll (std::string authenticationToken, GUID guid);
		int expungeTag (std::string authenticationToken, GUID guid);
		std::vector<SavedSearch*>* listSearches (std::string authenticationToken);
		SavedSearch* getSearch (std::string authenticationToken, GUID guid);
		SavedSearch* createSearch (std::string authenticationToken, 
			SavedSearch* search);
		int updateSearch (std::string authenticationToken, 
			SavedSearch* search);
		int expungeSearch (std::string authenticationToken, GUID guid);
		int findNoteOffset (std::string authenticationToken, NoteFilter* filter,
			GUID guid);
		NotesMetadataList* findNotesMetadata (std::string authenticationToken,
			NoteFilter* filter, int offset, int maxNotes,
			NotesMetadataResultSpec* resultSpec);
		NoteCollectionCounts* findNoteCounts (std::string authenticationToken,
			NoteFilter* filter, bool withTrash);
		Note* getNote (std::string authenticationToken,
			GUID guid, bool withContent, bool withResourcesData,
			bool withResourcesRecognition, bool withResourcesAlternateData);
		LazyMap* getNoteApplicationData (std::string authenticationToken,
			GUID guid);
		std::string getNoteAPplicationDataEntry (std::string authenticationToken,
			GUID guid, std::string key);
		int setNoteApplicationDataEntry (std::string authenticationToken,
			GUID guid, std::string key, std::string value);
		int unsetNoteApplicationDataEntry (std::string authenticationToken,
			GUID guid, std::string key);
		std::string getNoteContent (std::string authenticationToken,
			GUID guid);
		std::string getNoteSearchText (std::string authenticationToken,
			GUID guid, bool noteOnly, bool tokenizeForIndexing);
		std::string getResourceSearchText (std::string authenticationToken,
			GUID guid);
		std::vector<std::string>* getNoteTagNames (std::string authenticationToken,
			GUID guid);
		Note* createNote (std::string authenticationToken, Note* note);
		Note* updateNote (std::string authenticationToken, Note* note);
		int deleteNote (std::string authenticationToken, GUID guid);
		int expungeNote (std::string authenticationToken, GUID guid);
		int expungeNotes (std::string authenticationToken, 
			std::vector<int>* noteGuids);
		int expungeInactiveNotes (std::string authenticationToken);
		Note* copyNote (std::string authenticationToken, GUID noteGuid,
			GUID toNotebookGuid);
		std::vector<NoteVersionId*>* listNoteVersions (std::string authenticationToken,
			GUID noteGuid);
		Note* getNoteVersion (std::string authenticationToken, GUID noteGuid,
			int updateSequenceNum, bool withResourcesData, 
			bool withResourcesRecognition, bool withResourcesAlternateData);
		Resource* getResource (std::string authenticationToken, GUID guid,
			bool withData, bool withRecognition, bool withAttributes,
			bool withAlternateData);
		LazyMap* getResouceApplicationData (std::string authenticationToken,
			GUID guid);
		std::string getResourceApplicationDataEntry (std::string authenticationToken,
			GUID guid, std::string key);
		int setResourceApplicationDataEntry (std::string authenticationToken,
			GUID guid, std::string key, std::string value);
		int unsetResourceApplicationDataEntry (std::string authenticationToken,
			GUID guid, std::string key);
		int updateResource (std::string authenticationToken, Resource* resource);
		std::string getResourceData (std::string authenticationToken,
			GUID guid);
		Resource* getResourceByHash (std::string authenticationToken,
			GUID noteGuid, std::string contentHash, bool withdata,
			bool withRecognition, bool withAlternateData);
		std::string getResourceRecognition (std::string authenticationToken,
			GUID guid);
		std::string getResourceAlternateData (std::string authenticationToken,
			GUID guid);
		ResourceAttributes* getResourceAttributes (std::string authenticationToken,
			GUID guid);
		Notebook* getPublicNotebook (UserID* userId, std::string publicUri);
		SharedNotebook* createSharedNotebook (std::string authenticationToken,
			SharedNotebook* sharedNotebook);
		int updateSharedNotebook (std::string authenticationToken, 
			SharedNotebook* sharedNotebook);
		int setSharedNotebookRecipientSettings (std::string authenticationToken,
			long sharedNotebookId, SharedNotebookRecipientSettings* recipientSettings);
		int sendMessageToSharedNotebookMembers (std::string authenticationToken,
			GUID notebookGuid, std::string messageText, std::vector<std::string> recipients);
		std::vector<SharedNotebook*>* listSharedNotebooks (std::string authenticationToken);
		int expungeSharedNotebooks (std::string authenticationToken, 
			std::vector<long>* sharedNotebookIds);
		LinkedNotebook* createLinkedNotebook (std::string authenticationToken,
			LinkedNotebook* linkedNotebook);
		int updateLinkedNotebook (std::string authenticationToken,
			LinkedNotebook* linkedNotebook);
		std::vector<LinkedNotebook*> listLinkedNotebooks (std::string authenticationToken);
		int expungeLinkedNotebook (std::string authenticationToken,
			GUID guid);
		AuthenticateResult* authenticateToSharedNotebook (std::string shareKey,
			std::string authenticationToken);
		SharedNotebook* getSharedNotebookByAuth (std::string authenticationToken);
		void emailNote (std::string authenticationToken, NoteEmailParameters* parameters);
		std::string shareNote (std::string authenticationToken, 
			GUID guid);
		void stopSharingNote (std::string authenticationToken, GUID guid);
		AuthenticateResult* authenticateToSharedNote (std::string guid,
			std::string noteKey, std::string authenticationToken);
		RelatedResult* findRelated (std::string authenticationToken, 
			RelatedQuery* query, RelatedResultSpec* resultSpec);
	};

	/* USERSTORE TYPES */
	class AuthenticationResult {
	public:
		Timestamp* currentTime;
		std::string authenticationToken;
		Timestamp* expiration;
		User* ser;
		PublicUserInfo* publicUserInfo;
		std::string noteStoreUrl;
		std::string webApiUrlPrefix;
		bool secondFactorRequired;
		std::string secondFactorDeliveryHint;
	};

	class BootstrapSettings {
	public:
		std::string serviceHost;
		std::string marketingUrl;
		std::string supportUrl;
		std::string accountEmailDomain;
		bool enableFacebookSharing;
		bool enableGiftSubscriptions;
		bool enableSupportTickets;
		bool enableSharedNotebooks;
		bool enableSingleNoteSharing;
		bool enableSponsoredAccounts;
		bool enableTwitterSharing;
		bool enableLinkedInSharing;
		bool enablePublicNotebooks;
	};

	class BootstrapProfile {
	public:
		std::string name;
		BootstrapSettings* settings;
	};

	class BootstrapInfo {
	public:
		std::vector<BootstrapProfile*> profiles;
	};
	
	class PublicUserInfo {
	public:
		UserID* userId;
		std::string shardId;
		PrivilegeLevel privilege;
		std::string username;
		std::string noteStoreUrl;
		std::string webApiUrlPrefix;
	};

	class UserStore {
	private:
		evernote::edam::UserStoreClient* userStore;
		boost::shared_ptr<apache::thrift::transport::THttpClient> auth_http;
	public:
		UserStore (std::string evernoteUrl, int port, std::string parameterThree,
			std::string authenticationToken);
		~UserStore ();

		bool checkVersion (std::string clientName, short edamVersionMajor = 1,
			short edamVersionMinor = 15);
		BootstrapInfo* getBootstrapInfo (std::string locale);
		AuthenticateResult* authenticate (std::string username,
			std::string password, std::string consumerKey,
			std::string consumerSecret, bool supportsTwoFactor);
		AuthenticateResult* authenticateLongSession (std::string username,
			std::string password, std::string consumerKey, 
			std::string consumerSecret, std::string deviceIdentifier,
			std::string deviceDescription, bool supportsTwoFactor);
		AuthenticationResult* completeTwoFactorAuthentication (std::string 
			authenticationToken, std::string oneTimeCode, std::string deviceIdentifier,
			std::string deviceDescription);
		void revokeLongSession (std::string authenticationToken);
		AuthenticationResult* authenticateToBusiness (std::string authenticationToken);
		AuthenticationResult* refreshAuthentication (std::string authenticationToken);
		User* getUser (std::string authenticationToken);
		PublicUserInfo* getPublicUserInfo (std::string username);
		PremiumInfo* getPremiumInfo (std::string authenticationToken);
		std::string getNoteStoreUrl (std::string authenticationToken);
	};
}