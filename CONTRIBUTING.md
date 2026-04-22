# Contributing Guide — CyberCba2077

Welcome to the development workflow of **CyberCba2077**.

This document defines the standards and expectations for contributing to the project.  
The goal is to ensure code quality, maintainability, and an efficient review process.

All contributors are expected to follow these guidelines.

---

# Development Workflow

The project follows a **feature-driven development workflow** based on GitHub Issues and Pull Requests.

Every code change must follow this lifecycle:

1. A **task or bug is defined as a GitHub Issue**
2. A **developer implements the solution in a branch**
3. A **Pull Request (PR) is created**
4. The PR is reviewed by:
   - a **Tester**
   - a **Team Lead**
5. Once approved, the PR can be merged.

Direct commits to the main branch are not allowed.

---

# Issue Management

All work must be tracked using GitHub Issues.

Each Issue should contain:

- clear description of the problem or feature
- acceptance criteria
- relevant context
- expected behavior

Example Issue types:

- Feature
- Bug
- Refactor
- Documentation
- Technical debt

Pull Requests **must reference an Issue**.

Example:

```

Closes #12
Fixes #8
Related #21

```

---

# Branching Strategy

Every change must be implemented in a dedicated branch.

Recommended naming convention:

```

feature/<short-description>
bugfix/<short-description>
refactor/<short-description>

```

Examples:

```

feature/add-inventory-system
bugfix/fix-command-parser-crash
refactor/improve-player-ownership

```

Branches should remain **focused on a single logical change**.

Avoid mixing unrelated modifications.

---

# Pull Request Process

All changes must be delivered through **Pull Requests**.

A Pull Request must:

- reference the related Issue
- include a clear description of the change
- explain the motivation
- provide testing instructions
- include evidence when applicable

A good PR description should answer:

1. **What changed**
2. **Why the change was needed**
3. **How it was implemented**
4. **How reviewers can validate it**

---

# Required PR Checklist

Before requesting a review, the author must ensure:

- [ ] The PR references the related Issue
- [ ] The author is assigned to the PR
- [ ] The code compiles successfully
- [ ] The change has a clear and focused purpose
- [ ] The author reviewed their own diff
- [ ] No unrelated changes are included
- [ ] Temporary debugging code has been removed
- [ ] Naming is clear and consistent
- [ ] Documentation is updated if needed
- [ ] Tests are updated if applicable

---

# Code Review Policy

Every Pull Request must be reviewed before merging.

Required approvals:

- **Tester approval**
- **Team Lead approval**

The review process ensures:

- correctness
- maintainability
- readability
- architectural consistency

A PR cannot be merged without both approvals.

---

# Tester Responsibilities

The Tester is responsible for validating that:

- the feature behaves as expected
- the bug is correctly fixed
- the instructions to test the change are reproducible
- no regressions are introduced

The Tester must:

- follow the testing instructions in the PR
- report issues or unexpected behavior
- approve the PR only after validation

---

# Team Lead Responsibilities

The Team Lead is responsible for reviewing:

- architectural consistency
- code quality
- maintainability
- design decisions

The Team Lead ensures that:

- the solution aligns with project architecture
- code complexity is reasonable
- abstractions are appropriate
- the change does not introduce technical debt

---

# C++ Development Guidelines

Because this project is implemented in **C++**, contributors must follow good practices specific to the language.

Key principles:

### Prefer RAII

Resource ownership should be explicit and deterministic.

Avoid manual resource management when possible.

### Avoid unnecessary copying

Prefer:

```

const T&

```

when passing large objects.

### Respect const-correctness

Functions should declare `const` whenever possible.

### Use standard containers

Prefer STL containers (`std::vector`, `std::map`, etc.) over manual memory management.

### Keep functions small and focused

Functions should ideally perform **one clear responsibility**.

### Maintain modular design

Classes and modules should have well-defined responsibilities.

Avoid tight coupling between components.

---

# Pull Request Size Guidelines

Smaller PRs are easier to review and safer to merge.

Recommended guideline:

- Prefer **small, focused Pull Requests**
- Avoid PRs exceeding **~400 lines of changes** when possible

Large changes should be split into multiple PRs.

---

# Testing Expectations

All changes must be validated before merging.

Testing may include:

- unit tests
- manual validation
- scenario testing

The PR description must include **clear instructions to reproduce and verify the change**.

---

# Documentation

Code should be self-explanatory whenever possible.

However, documentation must be updated when:

- introducing new systems
- changing behavior
- modifying public interfaces

---

# Professional Conduct

Contributors are expected to behave professionally during the review process.

Guidelines:

- assume good intentions
- focus feedback on code, not people
- explain reasoning behind suggestions
- remain constructive

Code review is a **collaborative process aimed at improving the system**.

---

# Summary

To contribute successfully:

1. Work from a GitHub Issue
2. Implement changes in a dedicated branch
3. Open a Pull Request
4. Provide a clear description and testing steps
5. Obtain approvals from:
   - Tester
   - Team Lead
6. Merge only after validation

Following this workflow ensures a professional and maintainable development process.
