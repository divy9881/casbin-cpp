#pragma once

#include "pch.h"

#include <direct.h>
#include <algorithm>

#include <enforcer.h>
#include <util.h>

using namespace std;

namespace test_model_enforcer
{
	TEST_CLASS(TestModelEnforcer)
	{
		public:

			string filePath(string filepath) {
				char* root = _getcwd(NULL, 0);
				string rootStr = string(root);

				vector <string> directories = Split(rootStr, "\\", -1);
				vector<string>::iterator it = find(directories.begin(), directories.end(), "x64");
				vector <string> left{ *(it - 1) };
				it = find_end(directories.begin(), directories.end(), left.begin(), left.end());
				int index = int(directories.size() + (it - directories.end()));

				vector <string> finalDirectories(directories.begin(), directories.begin() + index + 1);

				vector<string> userD = Split(filepath, "/", -1);
				for (int i = 1; i < userD.size(); i++)
					finalDirectories.push_back(userD[i]);

				string filepath1 = finalDirectories[0];
				for (int i = 1; i < finalDirectories.size(); i++)
					filepath1 = filepath1 + "/" + finalDirectories[i];
				return filepath1;
			}

			Scope InitializeParams(string sub, string obj, string act) {
				Scope scope = InitializeScope();
				PushObject(scope, "r");
				PushStringPropToObject(scope, "r", sub, "sub");
				PushStringPropToObject(scope, "r", obj, "obj");
				PushStringPropToObject(scope, "r", act, "act");
				PushInt(scope, 3, "rlen");
				return scope;
			}

			Scope InitializeParamsWithoutUsers(string obj, string act) {
				Scope scope = InitializeScope();
				PushObject(scope, "r");
				PushStringPropToObject(scope, "r", obj, "obj");
				PushStringPropToObject(scope, "r", act, "act");
				PushInt(scope, 2, "rlen");
				return scope;
			}

			Scope InitializeParamsWithoutResources(string sub, string act) {
				Scope scope = InitializeScope();
				PushObject(scope, "r");
				PushStringPropToObject(scope, "r", sub, "sub");
				PushStringPropToObject(scope, "r", act, "act");
				PushInt(scope, 2, "rlen");
				return scope;
			}

			void TestEnforce(Enforcer* e, Scope scope, bool res) {
				Logger::WriteMessage(to_string(int(e->Enforce(scope))).c_str());
				Assert::AreEqual(res, e->Enforce(scope));
				Logger::WriteMessage("test passed.");
			}

			TEST_METHOD(TestBasicModel) {
				string model = filePath("../examples/basic_model.conf");
				string policy = filePath("../examples/basic_policy.csv");
				Enforcer* e = Enforcer :: NewEnforcer(model, policy);

				Scope scope;

				scope = InitializeParams("alice", "data1", "read");
				TestEnforce(e, scope, true);
				scope = InitializeParams("alice", "data1", "write");
				TestEnforce(e, scope, false);
				scope = InitializeParams("alice", "data2", "read");
				TestEnforce(e, scope, false);
				scope = InitializeParams("alice", "data2", "write");
				TestEnforce(e, scope, false);
				scope = InitializeParams("bob", "data1", "read");
				TestEnforce(e, scope, false);
				scope = InitializeParams("bob", "data1", "write");
				TestEnforce(e, scope, false);
				scope = InitializeParams("bob", "data2", "read");
				TestEnforce(e, scope, false);
				scope = InitializeParams("bob", "data2", "write");
				TestEnforce(e, scope, true);
			}
			
			TEST_METHOD(TestBasicModelWithoutSpaces) {
				string model = filePath("../examples/basic_model_without_spaces.conf");
				string policy = filePath("../examples/basic_policy.csv");
				Enforcer* e = Enforcer :: NewEnforcer(model, policy);

				Scope scope = InitializeParams("alice", "data1", "read");
				TestEnforce(e, scope, true);
				scope = InitializeParams("alice", "data1", "write");
				TestEnforce(e, scope, false);
				scope = InitializeParams("alice", "data2", "read");
				TestEnforce(e, scope, false);
				scope = InitializeParams("alice", "data2", "write");
				TestEnforce(e, scope, false);
				scope = InitializeParams("bob", "data1", "read");
				TestEnforce(e, scope, false);
				scope = InitializeParams("bob", "data1", "write");
				TestEnforce(e, scope, false);
				scope = InitializeParams("bob", "data2", "read");
				TestEnforce(e, scope, false);
				scope = InitializeParams("bob", "data2", "write");
				TestEnforce(e, scope, true);
			}

			TEST_METHOD(TestBasicModelWithRoot) {
				string model = filePath("../examples/basic_with_root_model.conf");
				string policy = filePath("../examples/basic_policy.csv");
				Enforcer* e = Enforcer::NewEnforcer(model, policy);

				Scope scope = InitializeParams("alice", "data1", "read");
				TestEnforce(e, scope, true);
				scope = InitializeParams("alice", "data1", "write");
				TestEnforce(e, scope, false);
				scope = InitializeParams("alice", "data2", "read");
				TestEnforce(e, scope, false);
				scope = InitializeParams("alice", "data2", "write");
				TestEnforce(e, scope, false);
				scope = InitializeParams("bob", "data1", "read");
				TestEnforce(e, scope, false);
				scope = InitializeParams("bob", "data1", "write");
				TestEnforce(e, scope, false);
				scope = InitializeParams("bob", "data2", "read");
				TestEnforce(e, scope, false);
				scope = InitializeParams("bob", "data2", "write");
				TestEnforce(e, scope, true);
				scope = InitializeParams("root", "data1", "read");
				TestEnforce(e, scope, true);
				scope = InitializeParams("root", "data1", "write");
				TestEnforce(e, scope, true);
				scope = InitializeParams("root", "data2", "read");
				TestEnforce(e, scope, true);
				scope = InitializeParams("root", "data2", "write");
				TestEnforce(e, scope, true);
			}

			TEST_METHOD(TestBasicModelWithoutUsers) {
				string model = filePath("../examples/basic_without_users_model.conf");
				string policy = filePath("../examples/basic_without_users_policy.csv");
				Enforcer* e = Enforcer::NewEnforcer(model, policy);

				Scope scope = InitializeParamsWithoutUsers("data1", "read");
				TestEnforce(e, scope, true);
				scope = InitializeParamsWithoutUsers("data1", "write");
				TestEnforce(e, scope, false);
				scope = InitializeParamsWithoutUsers("data2", "read");
				TestEnforce(e, scope, false);
				scope = InitializeParamsWithoutUsers("data2", "write");
				TestEnforce(e, scope, true);
			}

			TEST_METHOD(TestBasicModelWithoutResources) {
				string model = filePath("../examples/basic_without_resources_model.conf");
				string policy = filePath("../examples/basic_without_resources_policy.csv");
				Enforcer* e = Enforcer::NewEnforcer(model, policy);

				Scope scope = InitializeParamsWithoutResources("alice", "read");
				TestEnforce(e, scope, true);
				scope = InitializeParamsWithoutResources("alice", "write");
				TestEnforce(e, scope, false);
				scope = InitializeParamsWithoutResources("bob", "read");
				TestEnforce(e, scope, false);
				scope = InitializeParamsWithoutResources("bob", "write");
				TestEnforce(e, scope, true);
			}
			/*
			TEST_METHOD(TestRBACModel) {
				string model = filePath("../examples/rbac_model.conf");
				string policy = filePath("../examples/rbac_policy.csv");
				Enforcer* e = Enforcer::NewEnforcer(model, policy);

				Scope scope = InitializeParams("alice", "data1", "read");
				TestEnforce(e, scope, true);
				scope = InitializeParams("alice", "data1", "write");
				TestEnforce(e, scope, false);
				scope = InitializeParams("alice", "data2", "read");
				TestEnforce(e, scope, true);
				scope = InitializeParams("alice", "data2", "write");
				TestEnforce(e, scope, true);
				scope = InitializeParams("bob", "data1", "read");
				TestEnforce(e, scope, false);
				scope = InitializeParams("bob", "data1", "write");
				TestEnforce(e, scope, false);
				scope = InitializeParams("bob", "data1", "write");
				TestEnforce(e, scope, false);
				scope = InitializeParams("bob", "data1", "write");
				TestEnforce(e, scope, true);
			}

			TEST_METHOD(TestRBACModelWithResourceRoles) {
				string model = filePath("../examples/rbac_with_resource_roles_model.conf");
				string policy = filePath("../examples/rbac_with_resource_roles_policy.csv");
				Enforcer* e = Enforcer::NewEnforcer(model, policy);

				Scope scope = InitializeParams("alice", "data1", "read");
				TestEnforce(e, scope, true);
				scope = InitializeParams("alice", "data1", "write");
				TestEnforce(e, scope, true);
				scope = InitializeParams("alice", "data2", "read");
				TestEnforce(e, scope, false);
				scope = InitializeParams("alice", "data2", "write");
				TestEnforce(e, scope, true);
				scope = InitializeParams("bob", "data1", "read");
				TestEnforce(e, scope, false);
				scope = InitializeParams("bob", "data1", "write");
				TestEnforce(e, scope, false);
				scope = InitializeParams("bob", "data2", "read");
				TestEnforce(e, scope, false);
				scope = InitializeParams("bob", "data2", "write");
				TestEnforce(e, scope, true);
			}

			func TestRBACModelWithDomains(t* testing.T) {
				e, _ : = NewEnforcer("examples/rbac_with_domains_model.conf", "examples/rbac_with_domains_policy.csv")

					TestEnforce(e, "alice", "domain1", "data1", "read", true)
					TestEnforce(e, "alice", "domain1", "data1", "write", true)
					TestEnforce(e, "alice", "domain1", "data2", "read", false)
					TestEnforce(e, "alice", "domain1", "data2", "write", false)
					TestEnforce(e, "bob", "domain2", "data1", "read", false)
					TestEnforce(e, "bob", "domain2", "data1", "write", false)
					TestEnforce(e, "bob", "domain2", "data2", "read", true)
					TestEnforce(e, "bob", "domain2", "data2", "write", true)
			}

			func TestRBACModelWithDomainsAtRuntime(t* testing.T) {
				e, _ : = NewEnforcer("examples/rbac_with_domains_model.conf")

					e.AddPolicy("admin", "domain1", "data1", "read")
					e.AddPolicy("admin", "domain1", "data1", "write")
					e.AddPolicy("admin", "domain2", "data2", "read")
					e.AddPolicy("admin", "domain2", "data2", "write")

					e.AddGroupingPolicy("alice", "admin", "domain1")
					e.AddGroupingPolicy("bob", "admin", "domain2")

					TestEnforce(e, "alice", "domain1", "data1", "read", true)
					TestEnforce(e, "alice", "domain1", "data1", "write", true)
					TestEnforce(e, "alice", "domain1", "data2", "read", false)
					TestEnforce(e, "alice", "domain1", "data2", "write", false)
					TestEnforce(e, "bob", "domain2", "data1", "read", false)
					TestEnforce(e, "bob", "domain2", "data1", "write", false)
					TestEnforce(e, "bob", "domain2", "data2", "read", true)
					TestEnforce(e, "bob", "domain2", "data2", "write", true)

					// Remove all policy rules related to domain1 and data1.
					e.RemoveFilteredPolicy(1, "domain1", "data1")

					TestEnforce(e, "alice", "domain1", "data1", "read", false)
					TestEnforce(e, "alice", "domain1", "data1", "write", false)
					TestEnforce(e, "alice", "domain1", "data2", "read", false)
					TestEnforce(e, "alice", "domain1", "data2", "write", false)
					TestEnforce(e, "bob", "domain2", "data1", "read", false)
					TestEnforce(e, "bob", "domain2", "data1", "write", false)
					TestEnforce(e, "bob", "domain2", "data2", "read", true)
					TestEnforce(e, "bob", "domain2", "data2", "write", true)

					// Remove the specified policy rule.
					e.RemovePolicy("admin", "domain2", "data2", "read")

					TestEnforce(e, "alice", "domain1", "data1", "read", false)
					TestEnforce(e, "alice", "domain1", "data1", "write", false)
					TestEnforce(e, "alice", "domain1", "data2", "read", false)
					TestEnforce(e, "alice", "domain1", "data2", "write", false)
					TestEnforce(e, "bob", "domain2", "data1", "read", false)
					TestEnforce(e, "bob", "domain2", "data1", "write", false)
					TestEnforce(e, "bob", "domain2", "data2", "read", false)
					TestEnforce(e, "bob", "domain2", "data2", "write", true)
			}

			func TestRBACModelWithDomainsAtRuntimeMockAdapter(t* testing.T) {
			adapter: = fileadapter.NewAdapterMock("examples/rbac_with_domains_policy.csv")
				e, _ : = NewEnforcer("examples/rbac_with_domains_model.conf", adapter)

				e.AddPolicy("admin", "domain3", "data1", "read")
				e.AddGroupingPolicy("alice", "admin", "domain3")

				TestEnforce(e, "alice", "domain3", "data1", "read", true)

				TestEnforce(e, "alice", "domain1", "data1", "read", true)
				e.RemoveFilteredPolicy(1, "domain1", "data1")
				TestEnforce(e, "alice", "domain1", "data1", "read", false)

				TestEnforce(e, "bob", "domain2", "data2", "read", true)
				e.RemovePolicy("admin", "domain2", "data2", "read")
				TestEnforce(e, "bob", "domain2", "data2", "read", false)
			}

			func TestRBACModelWithDeny(t* testing.T) {
				e, _ : = NewEnforcer("examples/rbac_with_deny_model.conf", "examples/rbac_with_deny_policy.csv")

					TestEnforce(e, "alice", "data1", "read", true)
					TestEnforce(e, "alice", "data1", "write", false)
					TestEnforce(e, "alice", "data2", "read", true)
					TestEnforce(e, "alice", "data2", "write", false)
					TestEnforce(e, "bob", "data1", "read", false)
					TestEnforce(e, "bob", "data1", "write", false)
					TestEnforce(e, "bob", "data2", "read", false)
					TestEnforce(e, "bob", "data2", "write", true)
			}

			func TestRBACModelWithOnlyDeny(t* testing.T) {
				e, _ : = NewEnforcer("examples/rbac_with_not_deny_model.conf", "examples/rbac_with_deny_policy.csv")

					TestEnforce(e, "alice", "data2", "write", false)
			}

			func TestRBACModelWithCustomData(t* testing.T) {
				e, _ : = NewEnforcer("examples/rbac_model.conf", "examples/rbac_policy.csv")

					// You can add custom data to a grouping policy, Casbin will ignore it. It is only meaningful to the caller.
					// This feature can be used to store information like whether "bob" is an end user (so no subject will inherit "bob")
					// For Casbin, it is equivalent to: e.AddGroupingPolicy("bob", "data2_admin")
					e.AddGroupingPolicy("bob", "data2_admin", "custom_data")

					TestEnforce(e, "alice", "data1", "read", true)
					TestEnforce(e, "alice", "data1", "write", false)
					TestEnforce(e, "alice", "data2", "read", true)
					TestEnforce(e, "alice", "data2", "write", true)
					TestEnforce(e, "bob", "data1", "read", false)
					TestEnforce(e, "bob", "data1", "write", false)
					TestEnforce(e, "bob", "data2", "read", true)
					TestEnforce(e, "bob", "data2", "write", true)

					// You should also take the custom data as a parameter when deleting a grouping policy.
					// e.RemoveGroupingPolicy("bob", "data2_admin") won't work.
					// Or you can remove it by using RemoveFilteredGroupingPolicy().
					e.RemoveGroupingPolicy("bob", "data2_admin", "custom_data")

					TestEnforce(e, "alice", "data1", "read", true)
					TestEnforce(e, "alice", "data1", "write", false)
					TestEnforce(e, "alice", "data2", "read", true)
					TestEnforce(e, "alice", "data2", "write", true)
					TestEnforce(e, "bob", "data1", "read", false)
					TestEnforce(e, "bob", "data1", "write", false)
					TestEnforce(e, "bob", "data2", "read", false)
					TestEnforce(e, "bob", "data2", "write", true)
			}

			func TestRBACModelWithPattern(t* testing.T) {
				e, _ : = NewEnforcer("examples/rbac_with_pattern_model.conf", "examples/rbac_with_pattern_policy.csv")

					// Here's a little confusing: the matching function here is not the custom function used in matcher.
					// It is the matching function used by "g" (and "g2", "g3" if any..)
					// You can see in policy that: "g2, /book/:id, book_group", so in "g2()" function in the matcher, instead
					// of checking whether "/book/:id" equals the obj: "/book/1", it checks whether the pattern matches.
					// You can see it as normal RBAC: "/book/:id" == "/book/1" becomes KeyMatch2("/book/:id", "/book/1")
					e.rm.(*defaultrolemanager.RoleManager).AddMatchingFunc("KeyMatch2", util.KeyMatch2)
					TestEnforce(e, "alice", "/book/1", "GET", true)
					TestEnforce(e, "alice", "/book/2", "GET", true)
					TestEnforce(e, "alice", "/pen/1", "GET", true)
					TestEnforce(e, "alice", "/pen/2", "GET", false)
					TestEnforce(e, "bob", "/book/1", "GET", false)
					TestEnforce(e, "bob", "/book/2", "GET", false)
					TestEnforce(e, "bob", "/pen/1", "GET", true)
					TestEnforce(e, "bob", "/pen/2", "GET", true)

					// AddMatchingFunc() is actually setting a function because only one function is allowed,
					// so when we set "KeyMatch3", we are actually replacing "KeyMatch2" with "KeyMatch3".
					e.rm.(*defaultrolemanager.RoleManager).AddMatchingFunc("KeyMatch3", util.KeyMatch3)
					TestEnforce(e, "alice", "/book2/1", "GET", true)
					TestEnforce(e, "alice", "/book2/2", "GET", true)
					TestEnforce(e, "alice", "/pen2/1", "GET", true)
					TestEnforce(e, "alice", "/pen2/2", "GET", false)
					TestEnforce(e, "bob", "/book2/1", "GET", false)
					TestEnforce(e, "bob", "/book2/2", "GET", false)
					TestEnforce(e, "bob", "/pen2/1", "GET", true)
					TestEnforce(e, "bob", "/pen2/2", "GET", true)
			}

			type testCustomRoleManager struct {}

			func NewRoleManager() rbac.RoleManager{
				return &testCustomRoleManager{}
			}
				func(rm* testCustomRoleManager) Clear() error { return nil }
			func(rm* testCustomRoleManager) AddLink(name1 string, name2 string, domain ...string) error {
				return nil
			}
			func(rm* testCustomRoleManager) DeleteLink(name1 string, name2 string, domain ...string) error {
				return nil
			}
			func(rm* testCustomRoleManager) HasLink(name1 string, name2 string, domain ...string) (bool, error) {
				if name1 == "alice" && name2 == "alice" {
					return true, nil
				}
				else if name1 == "alice" && name2 == "data2_admin" {
					return true, nil
				}
				else if name1 == "bob" && name2 == "bob" {
					return true, nil
				}
				return false, nil
			}
			func(rm* testCustomRoleManager) GetRoles(name string, domain ...string) ([]string, error) {
				return[]string{}, nil
			}
			func(rm* testCustomRoleManager) GetUsers(name string, domain ...string) ([]string, error) {
				return[]string{}, nil
			}
			func(rm* testCustomRoleManager) PrintRoles() error { return nil }

			func TestRBACModelWithCustomRoleManager(t* testing.T) {
				e, _ : = NewEnforcer("examples/rbac_model.conf", "examples/rbac_policy.csv")
					e.SetRoleManager(NewRoleManager())
					e.LoadModel()
					_ = e.LoadPolicy()

					TestEnforce(e, "alice", "data1", "read", true)
					TestEnforce(e, "alice", "data1", "write", false)
					TestEnforce(e, "alice", "data2", "read", true)
					TestEnforce(e, "alice", "data2", "write", true)
					TestEnforce(e, "bob", "data1", "read", false)
					TestEnforce(e, "bob", "data1", "write", false)
					TestEnforce(e, "bob", "data2", "read", false)
					TestEnforce(e, "bob", "data2", "write", true)
			}

			type testResource struct {
				Name  string
					Owner string
			}

			func newTestResource(name string, owner string) testResource {
			r: = testResource{}
				r.Name = name
				r.Owner = owner
				return r
			}

			func TestABACModel(t* testing.T) {
				e, _ : = NewEnforcer("examples/abac_model.conf")

					data1 : = newTestResource("data1", "alice")
					data2 : = newTestResource("data2", "bob")

					TestEnforce(e, "alice", data1, "read", true)
					TestEnforce(e, "alice", data1, "write", true)
					TestEnforce(e, "alice", data2, "read", false)
					TestEnforce(e, "alice", data2, "write", false)
					TestEnforce(e, "bob", data1, "read", false)
					TestEnforce(e, "bob", data1, "write", false)
					TestEnforce(e, "bob", data2, "read", true)
					TestEnforce(e, "bob", data2, "write", true)
			}

			func TestKeyMatchModel(t* testing.T) {
				e, _ : = NewEnforcer("examples/keymatch_model.conf", "examples/keymatch_policy.csv")

					TestEnforce(e, "alice", "/alice_data/resource1", "GET", true)
					TestEnforce(e, "alice", "/alice_data/resource1", "POST", true)
					TestEnforce(e, "alice", "/alice_data/resource2", "GET", true)
					TestEnforce(e, "alice", "/alice_data/resource2", "POST", false)
					TestEnforce(e, "alice", "/bob_data/resource1", "GET", false)
					TestEnforce(e, "alice", "/bob_data/resource1", "POST", false)
					TestEnforce(e, "alice", "/bob_data/resource2", "GET", false)
					TestEnforce(e, "alice", "/bob_data/resource2", "POST", false)

					TestEnforce(e, "bob", "/alice_data/resource1", "GET", false)
					TestEnforce(e, "bob", "/alice_data/resource1", "POST", false)
					TestEnforce(e, "bob", "/alice_data/resource2", "GET", true)
					TestEnforce(e, "bob", "/alice_data/resource2", "POST", false)
					TestEnforce(e, "bob", "/bob_data/resource1", "GET", false)
					TestEnforce(e, "bob", "/bob_data/resource1", "POST", true)
					TestEnforce(e, "bob", "/bob_data/resource2", "GET", false)
					TestEnforce(e, "bob", "/bob_data/resource2", "POST", true)

					TestEnforce(e, "cathy", "/cathy_data", "GET", true)
					TestEnforce(e, "cathy", "/cathy_data", "POST", true)
					TestEnforce(e, "cathy", "/cathy_data", "DELETE", false)
			}

			func TestKeyMatch2Model(t* testing.T) {
				e, _ : = NewEnforcer("examples/keymatch2_model.conf", "examples/keymatch2_policy.csv")

					TestEnforce(e, "alice", "/alice_data", "GET", false)
					TestEnforce(e, "alice", "/alice_data/resource1", "GET", true)
					TestEnforce(e, "alice", "/alice_data2/myid", "GET", false)
					TestEnforce(e, "alice", "/alice_data2/myid/using/res_id", "GET", true)
			}

			func CustomFunction(key1 string, key2 string) bool{
				if key1 == "/alice_data2/myid/using/res_id" && key2 == "/alice_data/:resource" {
					return true
				}
			 else if key1 == "/alice_data2/myid/using/res_id" && key2 == "/alice_data2/:id/using/:resId" {
			  return true
		  }
	   else {
		return false
	}
			}

				func CustomFunctionWrapper(args ...interface {}) (interface {}, error) {
			key1: = args[0].(string)
				key2 : = args[1].(string)

				return bool(CustomFunction(key1, key2)), nil
			}

			func TestKeyMatchCustomModel(t* testing.T) {
				e, _ : = NewEnforcer("examples/keymatch_custom_model.conf", "examples/keymatch2_policy.csv")

					e.AddFunction("keyMatchCustom", CustomFunctionWrapper)

					TestEnforce(e, "alice", "/alice_data2/myid", "GET", false)
					TestEnforce(e, "alice", "/alice_data2/myid/using/res_id", "GET", true)
			}

			func TestIPMatchModel(t* testing.T) {
				e, _ : = NewEnforcer("examples/ipmatch_model.conf", "examples/ipmatch_policy.csv")

					TestEnforce(e, "192.168.2.123", "data1", "read", true)
					TestEnforce(e, "192.168.2.123", "data1", "write", false)
					TestEnforce(e, "192.168.2.123", "data2", "read", false)
					TestEnforce(e, "192.168.2.123", "data2", "write", false)

					TestEnforce(e, "192.168.0.123", "data1", "read", false)
					TestEnforce(e, "192.168.0.123", "data1", "write", false)
					TestEnforce(e, "192.168.0.123", "data2", "read", false)
					TestEnforce(e, "192.168.0.123", "data2", "write", false)

					TestEnforce(e, "10.0.0.5", "data1", "read", false)
					TestEnforce(e, "10.0.0.5", "data1", "write", false)
					TestEnforce(e, "10.0.0.5", "data2", "read", false)
					TestEnforce(e, "10.0.0.5", "data2", "write", true)

					TestEnforce(e, "192.168.0.1", "data1", "read", false)
					TestEnforce(e, "192.168.0.1", "data1", "write", false)
					TestEnforce(e, "192.168.0.1", "data2", "read", false)
					TestEnforce(e, "192.168.0.1", "data2", "write", false)
			}

			func TestGlobMatchModel(t* testing.T) {
				e, _ : = NewEnforcer("examples/glob_model.conf", "examples/glob_policy.csv")
					TestEnforce(e, "u1", "/foo/", "read", true)
					TestEnforce(e, "u1", "/foo", "read", false)
					TestEnforce(e, "u1", "/foo/subprefix", "read", true)
					TestEnforce(e, "u1", "foo", "read", false)

					TestEnforce(e, "u2", "/foosubprefix", "read", true)
					TestEnforce(e, "u2", "/foo/subprefix", "read", false)
					TestEnforce(e, "u2", "foo", "read", false)

					TestEnforce(e, "u3", "/prefix/foo/subprefix", "read", true)
					TestEnforce(e, "u3", "/prefix/foo/", "read", true)
					TestEnforce(e, "u3", "/prefix/foo", "read", false)

					TestEnforce(e, "u4", "/foo", "read", false)
					TestEnforce(e, "u4", "foo", "read", true)
			}

			func TestPriorityModel(t* testing.T) {
				e, _ : = NewEnforcer("examples/priority_model.conf", "examples/priority_policy.csv")

					TestEnforce(e, "alice", "data1", "read", true)
					TestEnforce(e, "alice", "data1", "write", false)
					TestEnforce(e, "alice", "data2", "read", false)
					TestEnforce(e, "alice", "data2", "write", false)
					TestEnforce(e, "bob", "data1", "read", false)
					TestEnforce(e, "bob", "data1", "write", false)
					TestEnforce(e, "bob", "data2", "read", true)
					TestEnforce(e, "bob", "data2", "write", false)
			}

			func TestPriorityModelIndeterminate(t* testing.T) {
				e, _ : = NewEnforcer("examples/priority_model.conf", "examples/priority_indeterminate_policy.csv")

					TestEnforce(e, "alice", "data1", "read", false)
			}

			func TestRBACModelInMultiLines(t* testing.T) {
				e, _ : = NewEnforcer("examples/rbac_model_in_multi_line.conf", "examples/rbac_policy.csv")

					TestEnforce(e, "alice", "data1", "read", true)
					TestEnforce(e, "alice", "data1", "write", false)
					TestEnforce(e, "alice", "data2", "read", true)
					TestEnforce(e, "alice", "data2", "write", true)
					TestEnforce(e, "bob", "data1", "read", false)
					TestEnforce(e, "bob", "data1", "write", false)
					TestEnforce(e, "bob", "data2", "read", false)
					TestEnforce(e, "bob", "data2", "write", true)
			}

			type testSub struct {
				Name string
					Age  int
			}

			func newTestSubject(name string, age int) testSub {
			s: = testSub{}
				s.Name = name
				s.Age = age
				return s
			}

			func TestABACPolicy(t* testing.T) {
				e, _ : = NewEnforcer("examples/abac_rule_model.conf", "examples/abac_rule_policy.csv")
					sub1 : = newTestSubject("alice", 16)
					sub2 : = newTestSubject("alice", 20)
					sub3 : = newTestSubject("alice", 65)

					TestEnforce(e, sub1, "/data1", "read", false)
					TestEnforce(e, sub1, "/data2", "read", false)
					TestEnforce(e, sub1, "/data1", "write", false)
					TestEnforce(e, sub1, "/data2", "write", true)
					TestEnforce(e, sub2, "/data1", "read", true)
					TestEnforce(e, sub2, "/data2", "read", false)
					TestEnforce(e, sub2, "/data1", "write", false)
					TestEnforce(e, sub2, "/data2", "write", true)
					TestEnforce(e, sub3, "/data1", "read", true)
					TestEnforce(e, sub3, "/data2", "read", false)
					TestEnforce(e, sub3, "/data1", "write", false)
					TestEnforce(e, sub3, "/data2", "write", false)
			}

			func TestCommentModel(t* testing.T) {
				e, _ : = NewEnforcer("examples/comment_model.conf", "examples/basic_policy.csv")

					TestEnforce(e, "alice", "data1", "read", true)
					TestEnforce(e, "alice", "data1", "write", false)
					TestEnforce(e, "alice", "data2", "read", false)
					TestEnforce(e, "alice", "data2", "write", false)
					TestEnforce(e, "bob", "data1", "read", false)
					TestEnforce(e, "bob", "data1", "write", false)
					TestEnforce(e, "bob", "data2", "read", false)
					TestEnforce(e, "bob", "data2", "write", true)
			}
			*/
	};
}