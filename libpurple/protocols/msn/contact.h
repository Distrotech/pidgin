/**
 * @file contact.h			Header file for contact.c
 *	Author
 * 		MaYuan<mayuan2006@gmail.com>
 * purple
 *
 * Purple is the legal property of its developers, whose names are too numerous
 * to list here.  Please refer to the COPYRIGHT file distributed with this
 * source distribution.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef _MSN_CONTACT_H_
#define _MSN_CONTACT_H_

#define MSN_CONTACT_SERVER	"omega.contacts.msn.com"

/* Get Contact List */

#define MSN_GET_CONTACT_POST_URL	"/abservice/SharingService.asmx"
#define MSN_GET_CONTACT_SOAP_ACTION "http://www.msn.com/webservices/AddressBook/FindMembership"
#define MSN_GET_CONTACT_UPDATE_XML "<View>Full</View>"\
	"<deltasOnly>true</deltasOnly>"\
	"<lastChange>%s</lastChange>"
#define MSN_GET_CONTACT_TEMPLATE	"<?xml version='1.0' encoding='utf-8'?>"\
"<soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\">"\
	"<soap:Header xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\">"\
		"<ABApplicationHeader xmlns=\"http://www.msn.com/webservices/AddressBook\">"\
			"<ApplicationId xmlns=\"http://www.msn.com/webservices/AddressBook\">09607671-1C32-421F-A6A6-CBFAA51AB5F4</ApplicationId>"\
			"<IsMigration xmlns=\"http://www.msn.com/webservices/AddressBook\">false</IsMigration>"\
			"<PartnerScenario xmlns=\"http://www.msn.com/webservices/AddressBook\">Initial</PartnerScenario>"\
		 "</ABApplicationHeader>"\
		"<ABAuthHeader xmlns=\"http://www.msn.com/webservices/AddressBook\">"\
		"<ManagedGroupRequest xmlns=\"http://www.msn.com/webservices/AddressBook\">false</ManagedGroupRequest>"\
		"</ABAuthHeader>"\
	"</soap:Header>"\
	"<soap:Body xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\">"\
		"<FindMembership xmlns=\"http://www.msn.com/webservices/AddressBook\">"\
			"<serviceFilter xmlns=\"http://www.msn.com/webservices/AddressBook\">"\
			"<Types xmlns=\"http://www.msn.com/webservices/AddressBook\">"\
				"<ServiceType xmlns=\"http://www.msn.com/webservices/AddressBook\">Messenger</ServiceType>"\
				"<ServiceType xmlns=\"http://www.msn.com/webservices/AddressBook\">Invitation</ServiceType>"\
				"<ServiceType xmlns=\"http://www.msn.com/webservices/AddressBook\">SocialNetwork</ServiceType>"\
				"<ServiceType xmlns=\"http://www.msn.com/webservices/AddressBook\">Space</ServiceType>"\
				"<ServiceType xmlns=\"http://www.msn.com/webservices/AddressBook\">Profile</ServiceType>"\
			"</Types>"\
			"</serviceFilter>"\
			"%s"\
		"</FindMembership>"\
	"</soap:Body>"\
"</soap:Envelope>"

/************************************************
 * Address Book SOAP
 * *********************************************/

#define MSN_ADDRESS_BOOK_POST_URL	"/abservice/abservice.asmx"

/* Create AddressBook template */
#define MSN_ADD_ADDRESSBOOK_SOAP_ACTION     "http://www.msn.com/webservices/AddressBook/ABAdd"

#define MSN_ADD_ADDRESSBOOK_TEMPLATE	"<?xml version=\"1.0\" encoding=\"utf-8\"?>"\
"<soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:soapenc=\"http://schemas.xmlsoap.org/soap/encoding/\">"\
	"<soap:Header>"\
		"<ABApplicationHeader xmlns=\"http://www.msn.com/webservices/AddressBook\">"\
			"<ApplicationId>09607671-1C32-421F-A6A6-CBFAA51AB5F4</ApplicationId>"\
			"<IsMigration>false</IsMigration>"\
			"<PartnerScenario>Initial</PartnerScenario>"\
		"</ABApplicationHeader>"\
		"<ABAuthHeader xmlns=\"http://www.msn.com/webservices/AddressBook\">"\
			"<ManagedGroupRequest>false</ManagedGroupRequest>"\
		"</ABAuthHeader>"\
	"</soap:Header>"\
	"<soap:Body>"\
		"<ABAdd xmlns=\"http://www.msn.com/webservices/AddressBook\">"\
			"<abInfo>"\
				"<name/>"\
				"<ownerPuid>0</ownerPuid>"\
				"<ownerEmail>%s</ownerEmail>"\
				"<fDefault>true</fDefault>"\
			"</abInfo>"\
		"</ABAdd>"\
	"</soap:Body>"\
"</soap:Envelope>"

/* Get AddressBook */
#define MSN_GET_ADDRESS_SOAP_ACTION	"http://www.msn.com/webservices/AddressBook/ABFindAll"
#define MSN_GET_ADDRESS_FULL_TIME	"0001-01-01T00:00:00.0000000-08:00"
#define MSN_GET_ADDRESS_UPDATE_XML "<deltasOnly>true</deltasOnly>"\
	"<lastChange>%s</lastChange>"

#define MSN_GET_GLEAM_UPDATE_XML \
	"%s"\
	"<dynamicItemView>Gleam</dynamicItemView>"\
	"<dynamicItemLastChange>%s</dynamicItemLastChange>"

#define MSN_GET_ADDRESS_TEMPLATE	"<?xml version=\"1.0\" encoding=\"utf-8\"?>"\
"<soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:soapenc=\"http://schemas.xmlsoap.org/soap/encoding/\">"\
	"<soap:Header>"\
		"<ABApplicationHeader xmlns=\"http://www.msn.com/webservices/AddressBook\">"\
			"<ApplicationId>09607671-1C32-421F-A6A6-CBFAA51AB5F4</ApplicationId>"\
			"<IsMigration>false</IsMigration>"\
			"<PartnerScenario>%s</PartnerScenario>"\
		"</ABApplicationHeader>"\
		"<ABAuthHeader xmlns=\"http://www.msn.com/webservices/AddressBook\">"\
			"<ManagedGroupRequest>false</ManagedGroupRequest>"\
		"</ABAuthHeader>"\
	"</soap:Header>"\
	"<soap:Body>"\
		"<ABFindAll xmlns=\"http://www.msn.com/webservices/AddressBook\">"\
			"<abId>00000000-0000-0000-0000-000000000000</abId>"\
			"<abView>Full</abView>"\
			"%s"\
		"</ABFindAll>"\
	"</soap:Body>"\
"</soap:Envelope>"


/*	Send this shit after adding a contact (with ABGroupContactAdd or something) damnit!	
<?xml version="1.0" encoding="utf-8"?>
<soap:Envelope xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:soapenc="http://schemas.xmlsoap.org/soap/encoding/">
	<soap:Header>
		<ABApplicationHeader xmlns="http://www.msn.com/webservices/AddressBook">
			<ApplicationId>09607671-1C32-421F-A6A6-CBFAA51AB5F4</ApplicationId>
			<IsMigration>false</IsMigration>
			<PartnerScenario>ContactSave</PartnerScenario>
		</ABApplicationHeader>
		<ABAuthHeader xmlns="http://www.msn.com/webservices/AddressBook">
			<ManagedGroupRequest>false</ManagedGroupRequest>
		</ABAuthHeader>
	</soap:Header>
	<soap:Body>
		<ABFindAll xmlns="http://www.msn.com/webservices/AddressBook">
			<abId>00000000-0000-0000-0000-000000000000</abId>
			<abView>Full</abView>
			<deltasOnly>true</deltasOnly>
			<lastChange>2007-08-22T06:19:36.84-07:00</lastChange>
			<dynamicItemView>Gleam</dynamicItemView>
			<dynamicItemLastChange>0001-01-01T00:00:00</dynamicItemLastChange>
		</ABFindAll>
	</soap:Body>
</soap:Envelope>
*/

/*Gleams SOAP request template*/
#define MSN_GET_GLEAMS_SOAP_ACTION "http://www.msn.com/webservices/AddressBook/ABFindAll"
#define MSN_GLEAMS_TEMPLATE "<?xml version=\"1.0\" encoding=\"utf-8\"?>"\
"<soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:soapenc=\"http://schemas.xmlsoap.org/soap/encoding/\">"\
	"<soap:Header>"\
		"<ABApplicationHeader xmlns=\"http://www.msn.com/webservices/AddressBook\">"\
			"<ApplicationId>09607671-1C32-421F-A6A6-CBFAA51AB5F4</ApplicationId>"\
			"<IsMigration>false</IsMigration>"\
			"<PartnerScenario>Initial</PartnerScenario>"\
		"</ABApplicationHeader>"\
		"<ABAuthHeader xmlns=\"http://www.msn.com/webservices/AddressBook\">"\
			"<ManagedGroupRequest>false</ManagedGroupRequest>"\
		"</ABAuthHeader>"\
	"</soap:Header>"\
	"<soap:Body>"\
		"<ABFindAll xmlns=\"http://www.msn.com/webservices/AddressBook\">"\
			"<abId>00000000-0000-0000-0000-000000000000</abId>"\
			"<abView>Full</abView>"\
			"<dynamicItemView>Gleam</dynamicItemView>"\
			"<dynamicItemLastChange>0001-01-01T00:00:00.0000000-08:00</dynamicItemLastChange>"\
		"</ABFindAll>"\
	"</soap:Body>"\
"</soap:Envelope>"


/*******************************************************
 * Contact Management SOAP actions
 *******************************************************/

/* Add a new contact t*/
#define MSN_CONTACT_ADD_SOAP_ACTION	"http://www.msn.com/webservices/AddressBook/ABContactAdd"
#define MSN_CONTACT_LIVE_PENDING_XML	"<Contact xmlns=\"http://www.msn.com/webservices/AddressBook\"><contactInfo><contactType>LivePending</contactType><passportName>%s</passportName><isMessengerUser>true</isMessengerUser></contactInfo></Contact>"

#define MSN_CONTACT_XML	"<Contact xmlns=\"http://www.msn.com/webservices/AddressBook\">"\
				"<contactInfo>"\
					"<passportName>%s</passportName>"\
					"<isSmtp>false</isSmtp>"\
					"<isMessengerUser>true</isMessengerUser>"\
				"</contactInfo>"\
			"</Contact>"

#define MSN_CONTACT_DISPLAYNAME_XML	"<Contact xmlns=\"http://www.msn.com/webservices/AddressBook\"><contactInfo><displayName>%s</displayName><passportName>%s</passportName><isMessengerUser>true</isMessengerUser></contactInfo></Contact>"

#define MSN_ADD_CONTACT_TEMPLATE	"<?xml version=\"1.0\" encoding=\"utf-8\"?><soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:soapenc=\"http://schemas.xmlsoap.org/soap/encoding/\"><soap:Header><ABApplicationHeader xmlns=\"http://www.msn.com/webservices/AddressBook\"><ApplicationId>09607671-1C32-421F-A6A6-CBFAA51AB5F4</ApplicationId><IsMigration>false</IsMigration><PartnerScenario>ContactSave</PartnerScenario></ABApplicationHeader><ABAuthHeader xmlns=\"http://www.msn.com/webservices/AddressBook\"><ManagedGroupRequest>false</ManagedGroupRequest></ABAuthHeader></soap:Header><soap:Body><ABContactAdd xmlns=\"http://www.msn.com/webservices/AddressBook\"><abId>00000000-0000-0000-0000-000000000000</abId><contacts>%s</contacts><options><EnableAllowListManagement>true</EnableAllowListManagement></options></ABContactAdd></soap:Body></soap:Envelope>"

/* Add a contact to a group */
#define MSN_ADD_CONTACT_GROUP_SOAP_ACTION	"http://www.msn.com/webservices/AddressBook/ABGroupContactAdd"
#define MSN_ADD_CONTACT_GROUP_TEMPLATE	"<?xml version=\"1.0\" encoding=\"utf-8\"?>"\
"<soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:soapenc=\"http://schemas.xmlsoap.org/soap/encoding/\">"\
	"<soap:Header>"\
		"<ABApplicationHeader xmlns=\"http://www.msn.com/webservices/AddressBook\">"\
			"<ApplicationId>09607671-1C32-421F-A6A6-CBFAA51AB5F4</ApplicationId>"\
			"<IsMigration>false</IsMigration>"\
			"<PartnerScenario>ContactSave</PartnerScenario>"\
		"</ABApplicationHeader>"\
		"<ABAuthHeader xmlns=\"http://www.msn.com/webservices/AddressBook\">"\
			"<ManagedGroupRequest>false</ManagedGroupRequest>"\
		"</ABAuthHeader>"\
	"</soap:Header>"\
	"<soap:Body>"\
		"<ABGroupContactAdd xmlns=\"http://www.msn.com/webservices/AddressBook\">"\
			"<abId>00000000-0000-0000-0000-000000000000</abId>"\
			"<groupFilter>"\
				"<groupIds>"\
					"<guid>%s</guid>"\
				"</groupIds>"\
			"</groupFilter>"\
			"<contacts>%s</contacts>"\
			"<groupContactAddOptions>"\
				"<fGenerateMissingQuickName>true</fGenerateMissingQuickName>"\
				"<EnableAllowListManagement>true</EnableAllowListManagement>"\
			"</groupContactAddOptions>"\
		"</ABGroupContactAdd>"\
	"</soap:Body>"\
"</soap:Envelope>"

/* Delete a contact from the Contact List */
#define MSN_CONTACT_DEL_SOAP_ACTION	"http://www.msn.com/webservices/AddressBook/ABContactDelete"
#define MSN_CONTACT_ID_XML		"<Contact><contactId>%s</contactId></Contact>"
#define MSN_DEL_CONTACT_TEMPLATE	"<?xml version=\"1.0\" encoding=\"utf-8\"?><soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:soapenc=\"http://schemas.xmlsoap.org/soap/encoding/\"><soap:Header><ABApplicationHeader xmlns=\"http://www.msn.com/webservices/AddressBook\"><ApplicationId>09607671-1C32-421F-A6A6-CBFAA51AB5F4</ApplicationId><IsMigration>false</IsMigration><PartnerScenario>Timer</PartnerScenario></ABApplicationHeader><ABAuthHeader xmlns=\"http://www.msn.com/webservices/AddressBook\"><ManagedGroupRequest>false</ManagedGroupRequest></ABAuthHeader></soap:Header><soap:Body><ABContactDelete xmlns=\"http://www.msn.com/webservices/AddressBook\"><abId>00000000-0000-0000-0000-000000000000</abId><contacts>%s</contacts></ABContactDelete></soap:Body></soap:Envelope>"

/* Remove a contact from a group */
#define MSN_CONTACT_DEL_GROUP_SOAP_ACTION	"http://www.msn.com/webservices/AddressBook/ABGroupContactDelete"
#define MSN_CONTACT_DEL_GROUP_TEMPLATE	"<?xml version=\"1.0\" encoding=\"utf-8\"?><soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:soapenc=\"http://schemas.xmlsoap.org/soap/encoding/\"><soap:Header><ABApplicationHeader xmlns=\"http://www.msn.com/webservices/AddressBook\"><ApplicationId>09607671-1C32-421F-A6A6-CBFAA51AB5F4</ApplicationId><IsMigration>false</IsMigration><PartnerScenario>Timer</PartnerScenario></ABApplicationHeader><ABAuthHeader xmlns=\"http://www.msn.com/webservices/AddressBook\"><ManagedGroupRequest>false</ManagedGroupRequest></ABAuthHeader></soap:Header><soap:Body><ABGroupContactDelete xmlns=\"http://www.msn.com/webservices/AddressBook\"><abId>00000000-0000-0000-0000-000000000000</abId><contacts>%s</contacts><groupFilter><groupIds><guid>%s</guid></groupIds></groupFilter></ABGroupContactDelete></soap:Body></soap:Envelope>"

#define MSN_MEMBER_TEMPLATE	"<Member xsi:type=\"PassportMember\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"><Type>Passport</Type><State>Accepted</State><PassportName>%s</PassportName></Member>"

/* Update Contact Nickname */
#define MSN_CONTACT_UPDATE_SOAP_ACTION	"http://www.msn.com/webservices/AddressBook/ABContactUpdate"
#define MSN_CONTACT_UPDATE_TEMPLATE	"<?xml version=\"1.0\" encoding=\"utf-8\"?>"\
"<soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:soapenc=\"http://schemas.xmlsoap.org/soap/encoding/\">"\
	"<soap:Header>"\
		"<ABApplicationHeader xmlns=\"http://www.msn.com/webservices/AddressBook\">"\
			"<ApplicationId>09607671-1C32-421F-A6A6-CBFAA51AB5F4</ApplicationId>"\
			"<IsMigration>false</IsMigration>"\
			"<PartnerScenario>Timer</PartnerScenario>"\
		"</ABApplicationHeader>"\
		"<ABAuthHeader xmlns=\"http://www.msn.com/webservices/AddressBook\">"\
			"<ManagedGroupRequest>false</ManagedGroupRequest>"\
		"</ABAuthHeader>"\
	"</soap:Header>"\
	"<soap:Body>"\
		"<ABContactUpdate xmlns=\"http://www.msn.com/webservices/AddressBook\">"\
			"<abId>00000000-0000-0000-0000-000000000000</abId>"\
			"<contacts>"\
				"<Contact xmlns=\"http://www.msn.com/webservices/AddressBook\">"\
					"<contactInfo>"\
						"<contactType>Me</contactType>"\
						"<displayName>%s</displayName>"\
					"</contactInfo>"\
					"<propertiesChanged>DisplayName</propertiesChanged>"\
				"</Contact>"\
			"</contacts>"\
		"</ABContactUpdate>"\
	"</soap:Body>"\
"</soap:Envelope>"


/*******************************************************
 * Contact Block/Unblock SOAP actions
 *******************************************************/

/* block means delete from allow list and add contact to block list */
#define MSN_SHARE_POST_URL		"/abservice/SharingService.asmx"
#define MSN_CONTACT_BLOCK_SOAP_ACTION	"http://www.msn.com/webservices/AddressBook/AddMember"

/* first delete contact from allow list */
#define MSN_CONTACT_DELECT_FROM_ALLOW_TEMPLATE "<?xml version=\"1.0\" encoding=\"utf-8\"?><soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:soapenc=\"http://schemas.xmlsoap.org/soap/encoding/\"><soap:Header><ABApplicationHeader xmlns=\"http://www.msn.com/webservices/AddressBook\"><ApplicationId>09607671-1C32-421F-A6A6-CBFAA51AB5F4</ApplicationId><IsMigration>false</IsMigration><PartnerScenario>BlockUnblock</PartnerScenario></ABApplicationHeader><ABAuthHeader xmlns=\"http://www.msn.com/webservices/AddressBook\"><ManagedGroupRequest>false</ManagedGroupRequest></ABAuthHeader></soap:Header><soap:Body><DeleteMember xmlns=\"http://www.msn.com/webservices/AddressBook\"><serviceHandle><Id>0</Id><Type>Messenger</Type><ForeignId></ForeignId></serviceHandle><memberships><Membership><MemberRole>Allow</MemberRole><Members><Member xsi:type=\"PassportMember\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"><Type>Passport</Type><MembershipId>%s</MembershipId><State>Accepted</State></Member></Members></Membership></memberships></DeleteMember></soap:Body></soap:Envelope>"

#define MSN_CONTACT_ADD_TO_BLOCK_TEMPLATE	"<?xml version=\"1.0\" encoding=\"utf-8\"?><soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:soapenc=\"http://schemas.xmlsoap.org/soap/encoding/\"><soap:Header><ABApplicationHeader xmlns=\"http://www.msn.com/webservices/AddressBook\"><ApplicationId>09607671-1C32-421F-A6A6-CBFAA51AB5F4</ApplicationId><IsMigration>false</IsMigration><PartnerScenario>BlockUnblock</PartnerScenario></ABApplicationHeader><ABAuthHeader xmlns=\"http://www.msn.com/webservices/AddressBook\"><ManagedGroupRequest>false</ManagedGroupRequest></ABAuthHeader></soap:Header><soap:Body><AddMember xmlns=\"http://www.msn.com/webservices/AddressBook\"><serviceHandle><Id>0</Id><Type>Messenger</Type><ForeignId></ForeignId></serviceHandle><memberships><Membership><MemberRole>Block</MemberRole><Members>%s</Members></Membership></memberships></AddMember></soap:Body></soap:Envelope>"

/* unblock means delete contact from block list */
#define MSN_CONTACT_UNBLOCK_SOAP_ACTION	"http://www.msn.com/webservices/AddressBook/DeleteMember"
#define MSN_UNBLOCK_CONTACT_TEMPLATE	"<?xml version=\"1.0\" encoding=\"utf-8\"?><soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:soapenc=\"http://schemas.xmlsoap.org/soap/encoding/\"><soap:Header><ABApplicationHeader xmlns=\"http://www.msn.com/webservices/AddressBook\"><ApplicationId>09607671-1C32-421F-A6A6-CBFAA51AB5F4</ApplicationId><IsMigration>false</IsMigration><PartnerScenario>BlockUnblock</PartnerScenario></ABApplicationHeader><ABAuthHeader xmlns=\"http://www.msn.com/webservices/AddressBook\"><ManagedGroupRequest>false</ManagedGroupRequest></ABAuthHeader></soap:Header><soap:Body><DeleteMember xmlns=\"http://www.msn.com/webservices/AddressBook\"><serviceHandle><Id>0</Id><Type>Messenger</Type><ForeignId></ForeignId></serviceHandle><memberships><Membership><MemberRole>Block</MemberRole><Members>%s</Members></Membership></memberships></DeleteMember></soap:Body></soap:Envelope>"


/*******************************************************
 * Group management SOAP actions
 *******************************************************/

/* add a group */
#define MSN_GROUP_ADD_SOAP_ACTION	"http://www.msn.com/webservices/AddressBook/ABGroupAdd"
#define MSN_GROUP_ADD_TEMPLATE	"<?xml version=\"1.0\" encoding=\"utf-8\"?><soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:soapenc=\"http://schemas.xmlsoap.org/soap/encoding/\"><soap:Header><ABApplicationHeader xmlns=\"http://www.msn.com/webservices/AddressBook\"><ApplicationId>09607671-1C32-421F-A6A6-CBFAA51AB5F4</ApplicationId><IsMigration>false</IsMigration><PartnerScenario>GroupSave</PartnerScenario></ABApplicationHeader><ABAuthHeader xmlns=\"http://www.msn.com/webservices/AddressBook\"><ManagedGroupRequest>false</ManagedGroupRequest></ABAuthHeader></soap:Header><soap:Body><ABGroupAdd xmlns=\"http://www.msn.com/webservices/AddressBook\"><abId>00000000-0000-0000-0000-000000000000</abId><groupAddOptions><fRenameOnMsgrConflict>false</fRenameOnMsgrConflict></groupAddOptions><groupInfo><GroupInfo><name>%s</name><groupType>C8529CE2-6EAD-434d-881F-341E17DB3FF8</groupType><fMessenger>false</fMessenger><annotations><Annotation><Name>MSN.IM.Display</Name><Value>1</Value></Annotation></annotations></GroupInfo></groupInfo></ABGroupAdd></soap:Body></soap:Envelope>"

/* delete a group */
#define MSN_GROUP_DEL_SOAP_ACTION	"http://www.msn.com/webservices/AddressBook/ABGroupDelete"
#define MSN_GROUP_DEL_TEMPLATE	"<?xml version=\"1.0\" encoding=\"utf-8\"?><soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:soapenc=\"http://schemas.xmlsoap.org/soap/encoding/\"><soap:Header><ABApplicationHeader xmlns=\"http://www.msn.com/webservices/AddressBook\"><ApplicationId>09607671-1C32-421F-A6A6-CBFAA51AB5F4</ApplicationId><IsMigration>false</IsMigration><PartnerScenario>Timer</PartnerScenario></ABApplicationHeader><ABAuthHeader xmlns=\"http://www.msn.com/webservices/AddressBook\"><ManagedGroupRequest>false</ManagedGroupRequest></ABAuthHeader></soap:Header><soap:Body><ABGroupDelete xmlns=\"http://www.msn.com/webservices/AddressBook\"><abId>00000000-0000-0000-0000-000000000000</abId><groupFilter><groupIds><guid>%s</guid></groupIds></groupFilter></ABGroupDelete></soap:Body></soap:Envelope>"

/* change a group's name */
#define MSN_GROUP_RENAME_SOAP_ACTION	"http://www.msn.com/webservices/AddressBook/ABGroupUpdate"
#define MSN_GROUP_RENAME_TEMPLATE	"<?xml version=\"1.0\" encoding=\"utf-8\"?><soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:soapenc=\"http://schemas.xmlsoap.org/soap/encoding/\"><soap:Header><ABApplicationHeader xmlns=\"http://www.msn.com/webservices/AddressBook\"><ApplicationId>09607671-1C32-421F-A6A6-CBFAA51AB5F4</ApplicationId><IsMigration>false</IsMigration><PartnerScenario>Timer</PartnerScenario></ABApplicationHeader><ABAuthHeader xmlns=\"http://www.msn.com/webservices/AddressBook\"><ManagedGroupRequest>false</ManagedGroupRequest></ABAuthHeader></soap:Header><soap:Body><ABGroupUpdate xmlns=\"http://www.msn.com/webservices/AddressBook\"><abId>00000000-0000-0000-0000-000000000000</abId><groups><Group><groupId>%s</groupId><groupInfo><name>%s</name></groupInfo><propertiesChanged>GroupName </propertiesChanged></Group></groups></ABGroupUpdate></soap:Body></soap:Envelope>"


typedef enum 
{
	MSN_ADD_BUDDY			= 0x01,
	MSN_ADD_BUDDY_TO_GROUP		= 0x02,
	MSN_REMOVE_BUDDY		= 0x04,
	MSN_REMOVE_BUDDY_FROM_GROUP	= 0x08,
	MSN_MOVE_BUDDY			= 0x10,
	MSN_ADD_GROUP			= 0x20,
	MSN_DEL_GROUP			= 0x40,
	MSN_RENAME_GROUP		= 0x80
} MsnCallbackAction;

typedef struct _MsnContact MsnContact;

struct _MsnContact
{
	MsnSession *session;

	MsnSoapConn *soapconn;
};

typedef struct _MsnCallbackState MsnCallbackState;

struct _MsnCallbackState
{
	gchar * who;
	gchar * old_group_name;
	gchar * new_group_name;
	gchar * guid;
	MsnListId list_id;
	MsnCallbackAction action;
};

typedef enum 
{
	MSN_AB_INITIAL,
	MSN_AB_SAVE_CONTACT
} MsnAddressBookAction;


/************************************************
 * function prototype
 ************************************************/
MsnContact * msn_contact_new(MsnSession *session);
void msn_contact_destroy(MsnContact *contact);

MsnCallbackState * msn_callback_state_new(void);
void msn_callback_state_free(MsnCallbackState *state);
void msn_callback_state_set_who(MsnCallbackState *state, const gchar *who);
void msn_callback_state_set_old_group_name(MsnCallbackState *state,
					   const gchar *old_group_name);
void msn_callback_state_set_new_group_name(MsnCallbackState *state, 
					   const gchar *new_group_name);
void msn_callback_state_set_guid(MsnCallbackState *state, const gchar *guid);
void msn_callback_state_set_list_id(MsnCallbackState *state, MsnListId list_id);
void msn_callback_state_set_action(MsnCallbackState *state, 
				   MsnCallbackAction action);

void msn_contact_connect(MsnContact *contact);
void msn_get_contact_list(MsnContact * contact, const char *update);
void msn_get_address_book(MsnContact *contact, MsnAddressBookAction abaction,
			  const char * update, const char * gupdate);

/*contact SOAP Operation*/
void msn_update_contact(MsnContact *contact, const char* nickname);

void msn_add_contact(MsnContact *contact, MsnCallbackState *state, 
		     const char *passport, const char *displayname);
void msn_delete_contact(MsnContact *contact, const char *contactId);

void msn_add_contact_to_group(MsnContact *contact, MsnCallbackState *state, 
			      const char *passport, const char *groupId);
void msn_del_contact_from_group(MsnContact *contact, const char *passport, 
				const char *group_name);
/*group operation*/
void msn_add_group(MsnSession *session, MsnCallbackState *state, 
					const char* group_name);
void msn_del_group(MsnSession *session, const gchar *group_name);
void msn_contact_rename_group(MsnSession *session, const char *old_group_name,
						   const char *new_group_name);


/*contact Block/unblock operation*/
void msn_block_contact(MsnContact *contact,const char* membership_id);
void msn_unblock_contact(MsnContact *contact,const char* passport);

void msn_contact_connect_init(MsnSoapConn *soapconn);

#endif /* _MSN_CONTACT_H_ */

